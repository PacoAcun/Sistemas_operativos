// ============================ p1/root.s ============================
.section .text
.global _start
.extern __stack_top

_start:
    b reset

reset:
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #0x13
    msr cpsr_c, r0

    mrs r0, cpsr
    bic r0, r0, #0x80
    msr cpsr_c, r0

    ldr sp, =__stack_top
    bl clear_bss
    bl main
loop:
    b loop

clear_bss:
    ldr r0, =__bss_start__
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
