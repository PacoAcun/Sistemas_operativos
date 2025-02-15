#include "uart.h"

/* Direcciones típicas para el UART0 en VersatilePB */
static volatile unsigned int * const UART0_DR = (unsigned int *)(0x101f1000 + 0x00);
static volatile unsigned int * const UART0_FR = (unsigned int *)(0x101f1000 + 0x18);

void uart_init(void)
{
    /* Enviar un caracter 'X' manualmente antes de configurar nada */
    *UART0_DR = 'X';
}

void uart_putc(char c)
{
    int limite = 100000;  /* Para evitar loops infinitos */
    /* Esperar a que el FIFO de TX no esté lleno (bit5 = TXFF) */
    while (((*UART0_FR) & (1 << 5)) && --limite) {
        /* Espera activa */
    }
    *UART0_DR = c;
}

void uart_puts(const char *str)
{
    while (*str != '\0') {
        uart_putc(*str);
        str++;
    }
}
