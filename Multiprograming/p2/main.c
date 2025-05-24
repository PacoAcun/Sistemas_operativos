// ============================ p2/main.c ============================
#include "../lib/os.h"
#include "../lib/stdio.h"

void main(void) {
    char letra = 'A';

    PRINT(">> P2 iniciado <<\n");

    while (1) {
        PRINT("%c\n", letra++);
        if (letra > 'Z') letra = 'A';
        for (volatile int i = 0; i < 30000000; i++);
    }                                
}
