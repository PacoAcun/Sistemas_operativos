// ============================ os.c ============================
#include "os.h"
#include "stdio.h"
#include <stdint.h>

#define DMTIMER2_BASE         0x48040000U
#define INTC_BASE             0x48200000U
#define CM_PER_BASE           0x44E00000U

#define TCLR                  (*(volatile uint32_t *)(DMTIMER2_BASE + 0x38))
#define TISR                  (*(volatile uint32_t *)(DMTIMER2_BASE + 0x28))
#define TIER                  (*(volatile uint32_t *)(DMTIMER2_BASE + 0x2C))
#define TCRR                  (*(volatile uint32_t *)(DMTIMER2_BASE + 0x3C))
#define TLDR                  (*(volatile uint32_t *)(DMTIMER2_BASE + 0x40))

#define INTC_MIR_CLEAR2       (*(volatile uint32_t *)(INTC_BASE  + 0xC8))
#define INTC_CONTROL          (*(volatile uint32_t *)(INTC_BASE  + 0x48))
#define INTC_ILR68            (*(volatile uint32_t *)(INTC_BASE  + 0x110))

#define CM_PER_TIMER2_CLKCTRL (*(volatile uint32_t *)(CM_PER_BASE + 0x80))

PCB     pcb_table[2];
uint32_t current_pid = 0;
static uint32_t svc_sp;  // Para guardar el SP del SVC al arrancar P1

extern uint32_t _stack_top;
extern uint32_t _stack_bottom;

extern void context_switch(uint32_t *old_sp, uint32_t new_sp);
extern void uart_init(void);
extern void timer_init(void);
extern void intc_init(void);
extern void enable_irq(void);
extern void pcb_init(void);

void timer_irq_handler(void) {
    // 1) limpiar bandera y señal IRQ
    TISR         = 0x2;
    INTC_CONTROL = 0x1;

    // 2) determinar prev/next
    uint32_t prev = current_pid;
    uint32_t next = (prev == 0 ? 1 : 0);

    // 3) mensaje sin prefijo, alineado a columna 0
    PRINT(">> INTERRUPCIÓN: PID=%d → %d\n", prev, next);

    // 4) actualizar antes del cambio
    current_pid = next;

    // 5) cambio de contexto
    context_switch(&pcb_table[prev].sp,
                   pcb_table[next].sp);
}

void os_main(void) {
    uart_init();

    PRINT("Stack OS: 0x%x - 0x%x\n", (uint32_t)&_stack_bottom, (uint32_t)&_stack_top);

    timer_init();
    intc_init();
    pcb_init();
    enable_irq();

    // breve retardo antes de arrancar P1
    for (volatile int i = 0; i < 10000000; i++);

    // arrancar P1 primero, sin salto extra
    PRINT(">> P1 iniciado <<\n");
    current_pid = 0;

    // guardar SP del SVC y cargar SP de P1
    context_switch(&svc_sp, pcb_table[0].sp);

    // si vuelve aquí, bloqueamos
    while (1) { }
}

void timer_init(void) {
    CM_PER_TIMER2_CLKCTRL = 0x2;
    while ((CM_PER_TIMER2_CLKCTRL & 0x3) != 0x2);

    TCLR = 0;
    TLDR = 0xFFFFFFFF - 24000000 + 1;  // overflow ~1 s
    TCRR = TLDR;
    TISR = 0x7;
    TIER = 0x2;
    TCLR = 0x3;

    INTC_MIR_CLEAR2 = (1 << 4);
    INTC_ILR68      = 0;

    // estos PRINT ya terminan en \n
    PRINT("Verificando si TCRR cambia...\n");
    uint32_t t1 = TCRR;
    for (volatile int i = 0; i < 1000000; i++);
    uint32_t t2 = TCRR;
    PRINT("TCRR inicial: 0x%x, después: 0x%x\n", t1, t2);
    PRINT((t1 != t2) ? "TCRR está contando.\n" : "TCRR no cambia.\n");
}

void intc_init(void) {
    INTC_CONTROL    = 0x2;
    INTC_MIR_CLEAR2 = (1 << 4);
    INTC_ILR68      = 0;
}

void pcb_init(void) {
    PRINT("Inicializando PCBs...\n");

    // P1
    pcb_table[0].sp          = 0x82110000;
    pcb_table[0].state       = 0;
    pcb_table[0].entry_point = 0x82100000;
    {
        uint32_t *stk = (uint32_t*)0x82110000;
        *(--stk) = 0x01000000;      // CPSR SVC
        *(--stk) = 0x82100000;      // PC
        for (int i = 0; i < 14; i++) *(--stk) = 0;
        pcb_table[0].sp = (uint32_t)stk;
    }

    // P2
    pcb_table[1].sp          = 0x82210000;
    pcb_table[1].state       = 0;
    pcb_table[1].entry_point = 0x82200000;
    {
        uint32_t *stk = (uint32_t*)0x82210000;
        *(--stk) = 0x01000000;      // CPSR SVC
        *(--stk) = 0x82200000;      // PC
        for (int i = 0; i < 14; i++) *(--stk) = 0;
        pcb_table[1].sp = (uint32_t)stk;
    }

    PRINT("PCBs listos.\n");
}
