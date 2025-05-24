#include "os.h"
#include "uart.h"  // Para usar uart_puts

/* Registros del DMTimer1 (AM335x)
 * Base: 0x44E31000
 * TLDR (valor de carga): offset 0x40
 * TCRR (valor actual): offset 0x3C
 * TCLR (control): offset 0x38
 * TSICR (limpia interrupción): offset 0x54
 * IRQSTATUS (estado de la interrupción): offset 0x28
 */
static volatile unsigned int * const T_LOAD   = (unsigned int *)(0x44E31000 + 0x40);
static volatile unsigned int * const T_VALUE  = (unsigned int *)(0x44E31000 + 0x3C);
static volatile unsigned int * const T_CTRL   = (unsigned int *)(0x44E31000 + 0x38);
static volatile unsigned int * const T_INTCLR = (unsigned int *)(0x44E31000 + 0x54);
static volatile unsigned int * const T_MIS    = (unsigned int *)(0x44E31000 + 0x28);

/* Registros del GIC (se usan para simular el antiguo VIC):
 * Distribuidor base típico: 0x48200000, CPU Interface base: 0x48201000
 * Registro para habilitar (enable): offset 0x100 del distribuidor
 * Registro de fin de interrupción (EOI): offset 0x10 de la interfaz CPU
 */
static volatile unsigned int * const VIC_ENABLE = (unsigned int *)(0x48200000 + 0x100);
static volatile unsigned int * const VIC_ADDR   = (unsigned int *)(0x48201000 + 0x10);

/* Variable global para callback de timer (opcional) */
void (*timer_callback)() = 0;

/*
 * Inicializa el timer para un número de segundos determinado.
 * Configura en modo periódico, habilita la interrupción, etc.
 */
void timer_init(int segs) 
{
    /* Cargar el valor inicial (ej. segs * 1,000,000 para 1MHz de clock) */
    *T_LOAD = segs * 1000000;

    /* Configurar el Timer.
       Se asume que:
         - bit7: habilitar el timer,
         - bit6: modo periódico,
         - bit5: habilitar la interrupción,
         - bit1: modo 32 bits,
         - bit0: auto-reload (wrap).
       (Consulta el TRM del AM335x para confirmar la configuración correcta). */
    *T_CTRL = (1u << 7) | (1u << 6) | (1u << 5) | (1u << 1) | (1u << 0);

    /* Habilitar la interrupción del timer en el GIC.
       Aquí se asume que el timer usa la línea de interrupción con ID 4.
       (Ajusta este valor según tu asignación de IRQ). */
    *VIC_ENABLE |= (1u << 4);

    /* Mensaje de depuración */
    if ((*VIC_ENABLE & (1u << 4)) != 0) {
        uart_puts("----(DEBUG) GIC_INTENABLE configurado correctamente----\n");
    } else {
        uart_puts("----(DEBUG) ERROR: GIC_INTENABLE no se configuró----\n");
    }
}

/*
 * Rutina llamada en la interrupción del timer.
 * Limpia la bandera de interrupción y notifica al GIC que se atendió la IRQ.
 * Se imprime un mensaje solo la primera vez que se dispara la IRQ.
 */
void timer_irq_handler(void)
{
    static int mensaje_impreso = 0;
    if (*T_MIS != 0) {
        if (!mensaje_impreso) {
            uart_puts("-------(7) Timer IRQ Fired!----\n");
            mensaje_impreso = 1;
        }
        *T_INTCLR = 1;  /* Limpiar la interrupción del timer */
    }
    *VIC_ADDR = 0;  /* Avisar al GIC que se atendió la IRQ (EOI) */
}

/*
 * Habilita las IRQs a nivel CPU, limpiando el bit de interrupción (I) en el CPSR.
 */
void irq_enable(void)
{
    asm volatile(
        "mrs   r0, cpsr    \n"
        "bic   r0, r0, #0x80\n"
        "msr   cpsr_c, r0  \n"
        :
        :
        : "r0"
    );
}

/*
 * Pequeña rutina de retardo por software (busy-wait),
 * útil para pruebas mínimas.
 */
void delay(int ciclos)
{
    while (ciclos--) {
        asm volatile("nop");
    }
}



