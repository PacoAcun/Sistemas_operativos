#include "stdio.h"
#include "os.h"
#include "string.h"
#include <stdarg.h>


void uint32_to_hex_str(uint32_t num, char *buffer, int buffer_size) {
    const char hex_digits[] = "0123456789ABCDEF";
    int i = 0;
    if (buffer_size < 9) return; // 8 digits + '\0'
    for (int shift = 28; shift >= 0; shift -= 4) {
        int digit = (num >> shift) & 0xF;
        buffer[i++] = hex_digits[digit];
    }
    buffer[i] = '\0';
}

void PRINT(const char *format, ...) {
    va_list args;
    va_start(args, format);
    const char *p = format;
    char buffer[32];

    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'x': {
                    uint32_t val = va_arg(args, uint32_t);
                    uint32_to_hex_str(val, buffer, sizeof(buffer));
                    uart_puts(buffer); // Removed extra "0x"
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    int_to_str(val, buffer, sizeof(buffer));
                    uart_puts(buffer);
                    break;
                }
                case 'c': {
                    char val = (char)va_arg(args, int);
                    uart_putc(val);
                    break;
                }
                default:
                    uart_putc(*p);
                    break;
            }
        } else {
            uart_putc(*p);
        }
        p++;
    }
    va_end(args);
}

// Función para leer entrada formateada
void READ(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const char *p = format;
    char buffer[32];

    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'd': {
                    int *val = va_arg(args, int *);
                    uart_gets_input(buffer, sizeof(buffer));
                    *val = str_to_int(buffer);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char *);
                    uart_gets_input(str, 32);
                    break;
                }
                default:
                    break;
            }
        }
        p++;
    }

    va_end(args);
}


