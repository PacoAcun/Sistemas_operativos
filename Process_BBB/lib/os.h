#ifndef OS_H
#define OS_H

#include <stdint.h>

/* Inicialización de UART0 */
void uart_init(void);

/* Envía un solo carácter */
void uart_putc(char c);

/* Envía una cadena terminada en '\0' */
void uart_puts(const char *str);

/* Lee del UART hasta '\n' o '\r' */
void uart_gets_input(char *buffer, int max_length);

/* Memory-mapped I/O utilities */
void PUT32(uint32_t addr, uint32_t value);
uint32_t GET32(uint32_t addr);

typedef struct {
    uint32_t pid;
    uint32_t sp;  // stack pointer del proceso
    uint32_t pc;  // dirección de inicio del proceso
} pcb_t;

extern pcb_t pcbs[2];
extern int current_pid;
extern int next_pid;


#endif // OS_H
