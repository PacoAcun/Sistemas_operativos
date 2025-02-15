.global _start
.global irq_handler

.section .text

/* 
 * Punto de entrada (_start).
 * Ajustamos SP, enviamos un caracter por UART como test, 
 * y llamamos a main().
 */
_start:
    LDR sp, =0x8000

    /* Escribir 'Z' en la UART como prueba rápida */
    LDR r1, =0x101f1000  @ Base de UART0 en VersatilePB
    MOV r2, #'Z'
    STR r2, [r1, #0x00]

    /* Llamar a main() */
    BL main

    /* Si main() termina, entrar a un bucle infinito */
    B .

/* Relleno de handlers para excepciones (stubs) */
reset_handler:
    B reset_handler

undefined_handler:
    B undefined_handler

swi_handler:
    B swi_handler

prefetch_handler:
    B prefetch_handler

abort_handler:
    B abort_handler

fiq_handler:
    B fiq_handler

/*
 * irq_handler:
 * - Ajustamos LR para que apunte a la instrucción que causó la IRQ,
 * - Guardamos registros en stack,
 * - Llamamos a la rutina de C (timer_irq_handler),
 * - Restauramos registros y retornamos de IRQ.
 */
irq_handler:
    SUB     lr, lr, #4
    STMFD   sp!, {r0-r12, lr}
    BL      timer_irq_handler
    LDMFD   sp!, {r0-r12, lr}
    SUBS    pc, lr, #4

