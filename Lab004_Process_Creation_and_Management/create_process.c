#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    printf("Parent Process: PID=%lu\n", GetCurrentProcessId());

    // Crear un nuevo proceso
    if (!CreateProcess(
            NULL,                 // Nombre del ejecutable (NULL si se usa un comando en lpCommandLine)
            "child.exe",          // Comando a ejecutar (debe existir el ejecutable child.exe en el directorio)
            NULL,                  // Seguridad del proceso
            NULL,                  // Seguridad del hilo
            FALSE,                 // Heredar manejos de proceso
            0,                     // Flags de creación
            NULL,                  // Variables de entorno
            NULL,                  // Directorio de trabajo actual
            &si,                   // Información de inicio
            &pi                    // Información del nuevo proceso
        )) {
        printf("Failed to create process. Error: %lu\n", GetLastError());
        return 1;
    }

    printf("Child Process: PID=%lu, Parent PID=%lu\n", pi.dwProcessId, GetCurrentProcessId());

    // Esperar a que el proceso hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Cerrar los manejos de proceso e hilo
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return 0;
}
