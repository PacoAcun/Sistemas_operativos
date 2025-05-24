#include "string.h"
#include "stdio.h"

void main() {
    int num1, num2, sum;

    PRINT("Programa: Sumar dos numeros\n");

    while (1) {
        // Pedir el primer número
        PRINT("Ingrese el primer numero: ");
        READ("%d", &num1);

        // Pedir el segundo número
        PRINT("Ingrese el segundo numero: ");
        READ("%d", &num2);

        // Calcular la suma
        sum = num1 + num2;

        // Mostrar el resultado
        PRINT("Suma: %d\n", sum);
    }
}



