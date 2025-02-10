#include "string.h"
#include "stdio.h"

void main() {
    char input1[16];
    char input2[16];
    int num1, num2, sum;

    uart_puts("Programa: Sumar dos numeros\n");

    while (1) {
        // Pedir el primer número
        uart_puts("Ingrese el primer numero: ");
        uart_gets(input1, sizeof(input1));
        num1 = str_to_int(input1);

        // Pedir el segundo número
        uart_puts("Ingrese el segundo numero: ");
        uart_gets(input2, sizeof(input2));
        num2 = str_to_int(input2);

        // Calcular la suma
        sum = num1 + num2;

        // Mostrar el resultado
        char result[32];
        int_to_str(sum, result, sizeof(result));
        uart_puts("Suma: ");
        uart_puts(result);
        uart_puts("\n");
    }
}



