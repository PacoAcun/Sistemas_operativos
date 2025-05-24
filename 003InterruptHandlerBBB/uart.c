#include <stdint.h>
#include "uart.h"

/* Base address de UART0 en AM335x (BeagleBone Black) */
#define UART0_BASE 0x44E09000U

/* Offsets de registros */
#define UART0_DR   (*(volatile uint32_t *)(UART0_BASE + 0x00))  /* Data Register */
#define UART0_FR   (*(volatile uint32_t *)(UART0_BASE + 0x18))  /* Flag Register */

/* Bit 5 = TX FIFO Full (TXFF) */
#define UART_FR_TXFF  (1 << 5)

void uart_init(void)
{
    /* Ya se supone que el arranque en assembly configuró pin‑mux, reloj y baudrate */
    UART0_DR = 'X';  /* Prueba rápida */
}

void uart_putc(char c)
{
    int limit = 100000;
    /* Esperar mientras TX FIFO esté lleno */
    while ((UART0_FR & UART_FR_TXFF) && --limit) { }
    UART0_DR = (uint32_t)c;
}

void uart_puts(const char *str)
{
    while (*str) {
        uart_putc(*str++);
    }
}

