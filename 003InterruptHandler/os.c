// os.c
#define UART0_BASE      0x101f1000
#define UART0_DR        (*((volatile unsigned int*)(UART0_BASE + 0x000)))
#define UART0_FR        (*((volatile unsigned int*)(UART0_BASE + 0x018)))
#define UART0_IBRD      (*((volatile unsigned int*)(UART0_BASE + 0x024)))
#define UART0_FBRD      (*((volatile unsigned int*)(UART0_BASE + 0x028)))
#define UART0_LCRH      (*((volatile unsigned int*)(UART0_BASE + 0x02C)))
#define UART0_CR        (*((volatile unsigned int*)(UART0_BASE + 0x030)))

void UARTInit(void) {
    // Deshabilitar UART
    UART0_CR = 0;

    // Configurar baudrate (115200)
    // UART clock = 24MHz, Baudrate = 115200
    UART0_IBRD = 13;    // 24MHz/(16 * 115200) = 13.0208
    UART0_FBRD = 1;     // Parte fraccional

    // 8 bits, 1 stop bit, sin paridad, FIFOs habilitados
    UART0_LCRH = (1<<4) | (1<<5) | (1<<6);

    // Habilitar UART, transmisión y recepción
    UART0_CR = (1<<0) | (1<<8) | (1<<9);
}

void UARTSendChar(char c) {
    // Esperar hasta que el FIFO de transmisión no esté lleno
    while(UART0_FR & (1<<5));
    UART0_DR = c;
}

void UARTSendString(const char *str) {
    for(; *str != '\0'; str++) {
        if(*str == '\n') {
            UARTSendChar('\r');
        }
        UARTSendChar(*str);
    }
}
