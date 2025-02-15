// main.c
#include "os.h"

int main(void) {
    // Inicializar UART
    UARTInit();
    
    // Mensaje inicial
    UARTSendString("Iniciando sistema...\n");
    
    int counter = 0;
    while(1) {
        UARTSendString("Test UART\n");
        
        // Delay simple
        for(volatile int i = 0; i < 5000000; i++);
        
        counter++;
    }
    
    return 0;
}