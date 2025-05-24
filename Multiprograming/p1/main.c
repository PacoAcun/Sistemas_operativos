// ============================ p1/main.c ============================
#include "../lib/os.h"
#include "../lib/stdio.h"

void main(void) {
    int c = 0;
    while (1) {
        PRINT("%d\n", c++);
        if (c >= 10) c = 0;
        for (volatile int i = 0; i < 30000000; i++);
    }
}
