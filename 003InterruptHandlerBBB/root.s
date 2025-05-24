.global _start
.global irq_handler

.section .text

_start:
    /* --- SP apuntando a DDR3 (Stack top) --- */
    LDR     sp, =0x80007FFF

    /* --- Habilitar reloj de UART0 (CM_WKUP_UART0_CLKCTRL) --- */
    LDR     r0, =0x44E004B4
    MOV     r1, #0x2
    STR     r1, [r0]

    /* --- Configurar pinmux UART0_TXD (CONTROL_MODULE_BASE + 0x974) --- */
    LDR     r0, =0x44E10000
    MOV     r1, #0x00      @ Modo0, sin pull-up/down
    STR     r1, [r0, #0x974]

    /* --- Inicializar UART0: 115200‑8N1 --- */
    LDR     r0, =0x44E09000 @ UART0 base
    MOV     r1, #0x83      @ LCR: DLAB=1, 8 bits, no parity
    STR     r1, [r0, #0x0C]
    MOV     r1, #26        @ DLL divisor para 115200
    STR     r1, [r0]
    MOV     r1, #0x00
    STR     r1, [r0, #4]   @ DLM=0
    MOV     r1, #0x03      @ LCR: DLAB=0, 8 bits, no parity
    STR     r1, [r0, #0x0C]

    /* --- Enviar 'Z' esperando THR empty (LSR bit5) --- */
wait_thr_empty:
    LDR     r2, [r0, #0x14] @ LSR
    TST     r2, #0x20
    BEQ     wait_thr_empty
    MOV     r1, #'Z'
    STR     r1, [r0]        @ THR

    BL      main
    B       .

irq_handler:
    SUB     lr, lr, #4
    STMFD   sp!, {r0-r12, lr}
    BL      timer_irq_handler
    LDMFD   sp!, {r0-r12, lr}
    SUBS    pc, lr, #4