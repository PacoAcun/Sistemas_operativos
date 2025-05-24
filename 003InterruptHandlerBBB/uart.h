#ifndef UART_H
#define UART_H

/* Prototipos para manejo sencillo de UART */
void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *str);

#endif /* UART_H */


