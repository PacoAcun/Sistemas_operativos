// ============================ os.h ============================
#ifndef OS_H
#define OS_H

#include <stdint.h>

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *str);
void uart_gets_input(char *buffer, int max_length);

void enable_irq(void);
void context_switch(uint32_t *save_sp, uint32_t restore_sp);
void timer_irq_handler(void);

typedef struct {
    uint32_t sp;           // Stack pointer
    uint32_t state;        // Estado
    uint32_t entry_point;  // Dirección de inicio del proceso
} PCB;

#define MAX_PROCESSES 2
extern PCB pcb_table[MAX_PROCESSES];
extern uint32_t current_pid;

#endif // OS_H