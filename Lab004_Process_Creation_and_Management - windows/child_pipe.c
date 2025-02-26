// child_pipe.c
#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    HANDLE hReadPipe;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    
    // Obtener el handle de la tubería del argumento
    if (argc != 2) {
        printf("Usage: %s <pipe_handle>\n", argv[0]);
        return 1;
    }
    
    // Convertir la cadena a handle
    sscanf(argv[1], "%p", &hReadPipe);
    
    // Leer el mensaje del padre
    if (ReadFile(hReadPipe, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
        printf("Child Process: Received \"%s\"\n", buffer);
    } else {
        printf("ReadFile failed (%lu)\n", GetLastError());
    }
    
    // Cerrar el handle
    CloseHandle(hReadPipe);
    
    return 0;
}