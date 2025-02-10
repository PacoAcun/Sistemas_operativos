#include "stdio.h"
#include <stdarg.h>

// External UART functions declared from existing code
extern void uart_putc(char c);
extern char uart_getc(void);
extern void uart_puts(const char* s);
extern void uart_gets_input(char* buffer, int max_length);
extern int uart_atoi(const char* s);
extern void uart_itoa(int num, char* buffer);

// Buffer for number conversion
static char num_buffer[32];

// Function to print a formatted string
void PRINT(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    int num = va_arg(args, int);
                    uart_itoa(num, num_buffer);
                    uart_puts(num_buffer);
                    break;
                }
                case 'f': {
                    // Simple float implementation (prints 2 decimal places)
                    double num = va_arg(args, double);
                    int whole = (int)num;
                    int decimal = (int)((num - whole) * 100);
                    if (decimal < 0) decimal = -decimal;
                    
                    uart_itoa(whole, num_buffer);
                    uart_puts(num_buffer);
                    uart_putc('.');
                    uart_itoa(decimal, num_buffer);
                    if (decimal < 10) uart_putc('0');
                    uart_puts(num_buffer);
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    uart_puts(str);
                    break;
                }
                default:
                    uart_putc(*format);
            }
        } else {
            uart_putc(*format);
        }
        format++;
    }
    
    va_end(args);
}

// Function to read formatted input
int READ(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[16];
    int items_read = 0;
    
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    int* num_ptr = va_arg(args, int*);
                    uart_gets_input(buffer, sizeof(buffer));
                    *num_ptr = uart_atoi(buffer);
                    items_read++;
                    break;
                }
                case 'f': {
                    float* float_ptr = va_arg(args, float*);
                    uart_gets_input(buffer, sizeof(buffer));
                    // Simple float conversion
                    *float_ptr = (float)uart_atoi(buffer);
                    items_read++;
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    uart_gets_input(str, 16); // Fixed size for simplicity
                    items_read++;
                    break;
                }
            }
        }
        format++;
    }
    
    va_end(args);
    return items_read;
}