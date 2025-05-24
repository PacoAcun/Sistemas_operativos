// ============================ os/root.s ============================
.section .vectors, "a"
.align 5
.global _start

_vectors:
    ldr pc, =_start
    .word 0
    .word 0
    .word 0
    .word 0 
    .word 0
    ldr pc, =irq_handler
    .word 0

.section .text
.global _start
.global irq_handler
.global enable_irq
.global context_switch
.extern _stack_top

_start:
    // IRQ stack (fijo si querés mantenerlo separado)
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #0x12
    msr cpsr_c, r0
    ldr sp, =0x82020000

    // SVC stack
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #0x13
    msr cpsr_c, r0
    ldr sp, =_stack_top

    ldr r0, =_vectors
    mcr p15, 0, r0, c12, c0, 0

    bl os_main
loop:
    b loop

irq_handler:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    mrs r0, spsr
    stmfd sp!, {r0}
    bl timer_irq_handler
    ldmfd sp!, {r0}
    msr spsr_cxsf, r0
    ldmfd sp!, {r0-r12, pc}^

enable_irq:
    mrs r0, cpsr
    bic r0, r0, #0x80
    msr cpsr_c, r0
    bx lr

context_switch:
    cpsid i
    stmfd sp!, {r0-r12, lr}
    mrs r2, spsr
    stmfd sp!, {r2}
    str sp, [r0]
    mov sp, r1
    ldmfd sp!, {r2}
    msr spsr_cxsf, r2
    ldmfd sp!, {r0-r12, lr}
    cpsie i
    bx lr
