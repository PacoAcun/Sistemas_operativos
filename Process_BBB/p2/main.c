
#include "../lib/os.h"
#include "../lib/stdio.h"

void main() {
    PRINT(">> P2 iniciado <<\n");

    char letra = 'A';
    while (1) {
        if (letra > 'Z') letra = 'A';

        PRINT("%c\n", letra);  // Usar PRINT en lugar de uart_putc para mejor depuración
        
        // Bucle de espera más corto para facilitar la depuración
        for (volatile int i = 0; i < 100000000; i++);
        letra++;
    }
}