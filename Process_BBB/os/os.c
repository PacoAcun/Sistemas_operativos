#include "../lib/os.h"
#include "../lib/stdio.h"

// Timer base DMTimer2 (usado para IRQ)
#define DMTIMER2_BASE       0x48040000U
#define DMTIMER_TIDR        (*(volatile uint32_t *)(DMTIMER2_BASE + 0x00))
#define DMTIMER_TIOCP_CFG   (*(volatile uint32_t *)(DMTIMER2_BASE + 0x10))
#define DMTIMER_TCLR        (*(volatile uint32_t *)(DMTIMER2_BASE + 0x38))
#define DMTIMER_TCRR        (*(volatile uint32_t *)(DMTIMER2_BASE + 0x3C))
#define DMTIMER_TLDR        (*(volatile uint32_t *)(DMTIMER2_BASE + 0x40))
#define DMTIMER_TIER        (*(volatile uint32_t *)(DMTIMER2_BASE + 0x2C))
#define DMTIMER_IRQSTATUS   (*(volatile uint32_t *)(DMTIMER2_BASE + 0x28))
#define DMTIMER_TSICR       (*(volatile uint32_t *)(DMTIMER2_BASE + 0x54))

// Registros del GIC
#define GIC_DIST_BASE         0x48200000U
#define GIC_DIST_CTRL         (*(volatile uint32_t *)(GIC_DIST_BASE + 0x000))
#define GIC_DIST_ENABLESET    (*(volatile uint32_t *)(GIC_DIST_BASE + 0x110))

#define GIC_CPU_BASE          0x48201000U
#define GIC_CPU_CTRL          (*(volatile uint32_t *)(GIC_CPU_BASE + 0x000))
#define GIC_CPU_PRIMASK       (*(volatile uint32_t *)(GIC_CPU_BASE + 0x004))
#define GIC_CPU_EOI           (*(volatile uint32_t *)(GIC_CPU_BASE + 0x010))

#define DMTIMER2_IRQ_ID       68
#define TIMER_CLOCK_HZ        24000000U

// PCBs
pcb_t pcbs[2];
int current_pid = 0;
int next_pid = 1;

// -----------------------------------------------------------------------------
// Timer Init
// -----------------------------------------------------------------------------
void timer_init_ms(int ms) {
    PRINT(">> Inicializando Timer...\n");

    unsigned int ticks = (TIMER_CLOCK_HZ / 1000) * ms;

    DMTIMER_TIOCP_CFG = 0x2;  // Enable module
    DMTIMER_TCLR = 0x0;       // Stop timer

    DMTIMER_TSICR = 0x2;      // Soft reset
    for (volatile int i = 0; i < 100000; i++);  // delay para completar reset

    DMTIMER_TLDR = 0xFFFFFFFF - ticks + 1;
    DMTIMER_TCRR = DMTIMER_TLDR;
    DMTIMER_TIER = 0x2;  // Habilita interrupción de overflow

    DMTIMER_TCLR = (1 << 0) | (1 << 1);  // Start + auto-reload

    PRINT(">> Timer configurado con %d ms\n", ms);

    // GIC config
    GIC_DIST_ENABLESET = (1 << (DMTIMER2_IRQ_ID % 32));
    GIC_DIST_CTRL = 1;
    GIC_CPU_PRIMASK = 0xF0;  // Este valor debería ser 0x00 si queremos aceptar todas las prioridades
    GIC_CPU_CTRL = 1;

    PRINT(">> GIC DIST_CTRL = 0x%x\n", GIC_DIST_CTRL);
    PRINT(">> GIC_CPU_CTRL = 0x%x\n", GIC_CPU_CTRL);
    PRINT(">> GIC_DIST_ENABLESET = 0x%x\n", GIC_DIST_ENABLESET);
}


// -----------------------------------------------------------------------------
// Habilitar IRQs a nivel CPU
// -----------------------------------------------------------------------------
void irq_enable() {
    PRINT(">> Habilitando IRQs a nivel CPU\n");
    asm volatile(
        "mrs r0, cpsr \n"
        "bic r0, r0, #0x80 \n"
        "msr cpsr_c, r0 \n"
        :
        :
        : "r0"
    );
}

// -----------------------------------------------------------------------------
// Handler llamado por root.s
// -----------------------------------------------------------------------------
void irq_handler() {
    PRINT(">> IRQ Timer recibida en OS <<\n");

    if (DMTIMER_IRQSTATUS & 0x2) {
        DMTIMER_IRQSTATUS = 0x2;  // limpiar IRQ

        int temp = current_pid;
        current_pid = next_pid;
        next_pid = temp;

        GIC_CPU_EOI = 0;

        PRINT(">> Contexto cambiado: ahora PID=%d\n", current_pid);
    } else {
        PRINT(">> IRQ recibida pero no del timer\n");
    }
}


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main() {
    PRINT(">> OS Iniciado <<\n");

    pcbs[0].pid = 0;
    pcbs[1].pid = 1;

    timer_init_ms(500);  // Interrupción cada 500 ms
    irq_enable();
}

