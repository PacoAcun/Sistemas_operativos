#include "os.h"
#include "uart.h"  // Para usar uart_puts

/* Direcciones de registro y offsets */
static volatile unsigned int * const T_LOAD   = (unsigned int *)(0x101E2000 + 0x00);
static volatile unsigned int * const T_VALUE  = (unsigned int *)(0x101E2000 + 0x04);
static volatile unsigned int * const T_CTRL   = (unsigned int *)(0x101E2000 + 0x08);
static volatile unsigned int * const T_INTCLR = (unsigned int *)(0x101E2000 + 0x0C);
static volatile unsigned int * const T_MIS    = (unsigned int *)(0x101E2000 + 0x10);

/* Registros del VIC */
static volatile unsigned int * const VIC_ENABLE = (unsigned int *)(0x10140000 + 0x10);
static volatile unsigned int * const VIC_ADDR   = (unsigned int *)(0x10140000 + 0x30);

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

    /* Configuración del Timer:
       bit7=enable, bit6=periodic, bit5=interrupt enable,
       bit1=32-bit, bit0=wrap (auto-reload) */
    *T_CTRL = (1u << 7) | (1u << 6) | (1u << 5) | (1u << 1) | (1u << 0);

    /* Habilitar la línea de interrupción #4 en el VIC */
    *VIC_ENABLE |= (1u << 4);

    /* Mensaje de depuración (impreso solo una vez en la inicialización) */
    if ((*VIC_ENABLE & (1u << 4)) != 0) {
        uart_puts("----(DEBUG) VIC_INTENABLE configurado correctamente----\n");
    } else {
        uart_puts("----(DEBUG) ERROR: VIC_INTENABLE no se configuro----\n");
    }
}

/*
 * Rutina llamada en la interrupción de timer.
 * Limpia la bandera de interrupción y notifica al VIC que terminó.
 * El mensaje se imprime solo la primera vez que se dispara la IRQ.
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
    *VIC_ADDR = 0;  /* Avisar al VIC que se atendió la IRQ */
}

/*
 * Habilita las IRQs a nivel CPU, limpiando el bit de interrupción en CPSR.
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


