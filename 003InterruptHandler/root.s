// root.s
.section .vectors, "ax"
.global _start

_start:
    b reset_handler    // Reset
    b .               // Undefined instruction
    b .               // Software interrupt
    b .               // Prefetch abort
    b .               // Data abort
    b .               // Reserved
    b .               // IRQ
    b .               // FIQ

reset_handler:
    // Set up stack pointer
    ldr sp, =0x18000

    // Clear BSS section
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
bss_clear:
    cmp r0, r1
    beq bss_clear_done
    str r2, [r0], #4
    b bss_clear
bss_clear_done:

    // Jump to main
    bl main

    // Loop forever if main returns
1:  b 1b
