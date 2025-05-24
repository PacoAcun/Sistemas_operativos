.section .text
.global _start

_start:
b reset

reset:
    mrs r0, cpsr
    bic r0, r0, #0x1F       // borrar bits de modo actuales
    orr r0, r0, #0x13       // establecer modo SVC (0b10011)
    msr cpsr_c, r0

    // Modificar para usar CPSR correcto que permita IRQs
    mrs r0, cpsr
    bic r0, r0, #0x80       // Habilitar IRQ (bit 7)
    msr cpsr_c, r0

    ldr sp, =0x82110000     // Configurar stack del proceso
    bl clear_bss            // Limpiar .bss
    bl main                 // Llamar a main()
loop:
    b loop                  // Bucle infinito (proceso nunca termina)

clear_bss:
    ldr r0, =__bss_start__  // Limpiar .bss como en os/root.s
    ldr r1, =__bss_end__
    mov r2, #0
bss_clear:
    cmp r0, r1
    strlt r2, [r0], #4
    blt bss_clear
    bx lr

.globl PUT32
PUT32:
    str r1, [r0]
    bx lr

.globl GET32
GET32:
    ldr r0, [r0]
    bx lr
