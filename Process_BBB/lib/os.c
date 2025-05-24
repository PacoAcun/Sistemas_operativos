
#include "os.h"

/* Base address de UART0 en AM335x (BeagleBone Black) */
#define UART0_BASE 0x44E09000U

/* Punteros a registros (offsets en bytes) */
#define UART0_RBR  (*(volatile uint32_t *)(UART0_BASE + 0x00))  /* Receive Buffer Register (lectura) */
#define UART0_THR  (*(volatile uint32_t *)(UART0_BASE + 0x00))  /* Transmit Holding Register (escritura) */
#define UART0_DLL  (*(volatile uint32_t *)(UART0_BASE + 0x00))  /* Divisor Latch Low */
#define UART0_DLH  (*(volatile uint32_t *)(UART0_BASE + 0x04))  /* Divisor Latch High */
#define UART0_IER  (*(volatile uint32_t *)(UART0_BASE + 0x04))  /* Interrupt Enable Register */
#define UART0_FCR  (*(volatile uint32_t *)(UART0_BASE + 0x08))  /* FIFO Control Register */
#define UART0_LCR  (*(volatile uint32_t *)(UART0_BASE + 0x0C))  /* Line Control Register */
#define UART0_MCR  (*(volatile uint32_t *)(UART0_BASE + 0x10))  /* Modem Control Register */
#define UART0_LSR  (*(volatile uint32_t *)(UART0_BASE + 0x14))  /* Line Status Register */
#define UART0_FR   (*(volatile uint32_t *)(UART0_BASE + 0x18))  /* Flag Register (TI specific) */

/* Bits para registros */
#define UART_FR_TXFF  (1U << 5)  /* TX FIFO Full */
#define UART_FR_RXFE  (1U << 4)  /* RX FIFO Empty */
#define UART_LSR_THRE (1U << 5)  /* Transmitter Holding Register Empty */
#define UART_LSR_DR   (1U << 0)  /* Data Ready */
#define UART_LCR_DLAB (1U << 7)  /* Divisor Latch Access Bit */

void uart_init(void) {
    /* Configuración básica de UART usando LSR en lugar de FR para compatibilidad */
    /* Nota: U-Boot ya ha configurado el UART, pero lo reiniciamos para mayor seguridad */
    
    /* Escribe 'X' como mensaje de inicio para depuración */
    UART0_THR = 'X';
    
    /* Asegúrate de que el THR esté vacío antes de continuar */
    while ((UART0_LSR & UART_LSR_THRE) == 0) { }
}

void uart_putc(char c) {
    /* Espera hasta que el THR esté vacío (usando LSR en lugar de FR) */
    while ((UART0_LSR & UART_LSR_THRE) == 0) { }
    UART0_THR = (uint32_t)c;
}

void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;
    while (i < max_length - 1) {
        /* Espera por dato (usando LSR en lugar de FR) */
        while ((UART0_LSR & UART_LSR_DR) == 0) { }
        c = (char)UART0_RBR;
        /* Eco */
        uart_putc(c);
        if (c == '\n' || c == '\r') {
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

