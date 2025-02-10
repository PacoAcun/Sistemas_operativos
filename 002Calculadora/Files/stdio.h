#ifndef STDIO_H
#define STDIO_H

// Format and print a string to UART
void PRINT(const char *format, ...);

// Read and format input from UART
int READ(const char *format, ...);

#endif // STDIO_H
