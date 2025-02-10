#include "string.h"

// Convertir cadena a entero
int str_to_int(const char *s) {
    int num = 0;
    int sign = 1;

    // Manejar signo opcional
    if (*s == '-') {
        sign = -1;
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        num = num * 10 + (*s - '0');
        s++;
    }

    return sign * num;
}

// Convertir entero a cadena
void int_to_str(int num, char *buffer, int buffer_size) {
    int i = 0;
    int is_negative = 0;

    // Manejar cero explícitamente
    if (num == 0) {
        if (buffer_size > 1) {
            buffer[i++] = '0';
            buffer[i] = '\0';
        }
        return;
    }

    // Manejar signo negativo
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Generar dígitos en orden inverso
    while (num != 0 && i < buffer_size - 1) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Añadir signo negativo si es necesario
    if (is_negative && i < buffer_size - 1) {
        buffer[i++] = '-';
    }

    // Terminar cadena
    buffer[i] = '\0';

    // Invertir cadena
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}
