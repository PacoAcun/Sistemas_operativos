.section .vectors, "a"
.align 5
.global _vectors

_vectors:
    ldr pc, =_start          // Reset
    ldr pc, =halt            // Undefined
    ldr pc, =halt            // SWI
    ldr pc, =halt            // Prefetch abort
    ldr pc, =halt            // Data abort
    nop                      // Reserved
    ldr pc, =irq_handler_entry // IRQ
    ldr pc, =halt            // FIQ

halt: b .

.section .text
.global _start
.global irq_handler_entry
.extern main
.extern pcbs
.extern current_pid
.extern next_pid
.extern irq_handler

_start:
    // stack del sistema operativo
    ldr sp, =0x82010000

    // configurar VBAR
    ldr r0, =_vectors
    mcr p15, 0, r0, c12, c0, 0

    // inicializar PCBs
    ldr r0, =pcbs

    // PCB 0 - p1
    ldr r1, =0x82110000    // stack top
    str r1, [r0, #4]       // sp
    ldr r1, =0x82100000    // pc
    str r1, [r0, #8]       // pc
    mov r1, #0             // pid
    str r1, [r0]           // pid

    // PCB 1 - p2
    add r0, r0, #12
    ldr r1, =0x82220000
    str r1, [r0, #4]       // sp
    ldr r1, =0x82200000
    str r1, [r0, #8]       // pc
    mov r1, #1
    str r1, [r0]           // pid

    bl main

    // Simular salto como si fuera por IRQ (igual que en irq_handler_entry)
    ldr r0, =current_pid
    mov r1, #0
    str r1, [r0]

    ldr r2, =pcbs
    mov r3, #12
    mul r1, r1, r3
    add r2, r2, r1
    ldr sp, [r2, #4]
    ldr r1, [r2, #8]

    mov lr, r1
    subs pc, lr, #4





irq_handler_entry:
    // guardar contexto de proceso actual
    ldr r0, =current_pid
    ldr r1, [r0]
    ldr r2, =pcbs
    mov r3, #12
    mul r1, r1, r3
    add r2, r2, r1
    str sp, [r2]          // guardar SP

    bl irq_handler        // cambia current_pid

    // cargar SP del siguiente proceso
    ldr r0, =current_pid
    ldr r1, [r0]
    ldr r2, =pcbs
    mov r3, #12
    mul r1, r1, r3
    add r2, r2, r1
    ldr sp, [r2]          // restaurar SP
    ldr r1, [r2, #4]      // pc

    mov lr, r1
    subs pc, lr, #4       // simulate return


