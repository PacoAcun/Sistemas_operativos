#ifndef STDIO_H
#define STDIO_H

void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc();
void uart_gets(char *buffer, int max_length);

#endif // STDIO_H

