#ifndef OS_H
#define OS_H

/* Prototipos de funciones */
void timer_init(int segs);
void irq_enable(void);
void timer_irq_handler(void);
void delay(int ciclos);

/* Callback opcional para el timer */
extern void (*timer_callback)();

#endif /* OS_H */
