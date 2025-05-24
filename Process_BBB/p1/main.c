#include "../lib/os.h"
#include "../lib/stdio.h"

void main() {
    PRINT(">> p1 iniciado <<\n");  // debug

    int a = 0;
    while (1) {
        if (a == 10) a = 0;

        PRINT("%d\n", a);  // Usar PRINT en lugar de uart_putc para mejor depuración
        
        // Bucle de espera más corto para facilitar la depuración
        for (volatile int i = 0; i < 100000000; i++);
        a++;
    }
}








