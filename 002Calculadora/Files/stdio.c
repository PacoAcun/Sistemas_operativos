#include "stdio.h"

#define UART0_BASE  0x101f1000
#define UART_DR     0x00
#define UART_FR     0x18
#define UART_FR_TXFF 0x20
#define UART_FR_RXFE 0x10

volatile unsigned int * const UART0 = (unsigned int *)UART0_BASE;

// Enviar un solo carácter por UART
void uart_putc(char c) {
    while (UART0[UART_FR / 4] & UART_FR_TXFF);
    UART0[UART_DR / 4] = c;
}

// Recibir un solo carácter por UART
char uart_getc() {
    while (UART0[UART_FR / 4] & UART_FR_RXFE);
    return (char)(UART0[UART_DR / 4] & 0xFF);
}

// Enviar una cadena por UART
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Recibir una cadena desde UART
void uart_gets(char *buffer, int max_length) {
    int i = 0;
    char c;
    while (i < max_length - 1) { // Deja espacio para el terminador nulo
        c = uart_getc();
        if (c == '\n' || c == '\r') {
            uart_putc('\n'); // Eco de nueva línea
            break;
        }
        uart_putc(c); // Eco del carácter ingresado
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Terminar cadena
}

