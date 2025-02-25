// parent_shmem.c
#include <windows.h>
#include <stdio.h>

#define SHARED_MEM_SIZE 1024
#define SHARED_MEM_NAME "Local\\SharedMemoryExample"

int main() {
    HANDLE hMapFile;
    LPVOID pBuf;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    TCHAR cmdLine[] = TEXT("child_shmem.exe");
    char message[] = "Shared Memory Example";
    
    // Crear objeto de mapeo de archivo
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // Usar paginación de archivo
        NULL,                    // Atributos de seguridad por defecto
        PAGE_READWRITE,          // Acceso de lectura/escritura
        0,                       // Tamaño máximo del objeto (alto)
        SHARED_MEM_SIZE,         // Tamaño máximo del objeto (bajo)
        SHARED_MEM_NAME);        // Nombre del objeto de mapeo
    
    if (hMapFile == NULL) {
        printf("CreateFileMapping failed (%lu)\n", GetLastError());
        return 1;
    }
    
    // Obtener vista del archivo mapeado
    pBuf = MapViewOfFile(
        hMapFile,                // Handle al objeto de mapeo
        FILE_MAP_ALL_ACCESS,     // Permiso de lectura/escritura
        0,                       // Offset alto
        0,                       // Offset bajo
        SHARED_MEM_SIZE);        // Número de bytes a mapear
    
    if (pBuf == NULL) {
        printf("MapViewOfFile failed (%lu)\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }
    
    // Escribir mensaje en memoria compartida
    printf("Parent Process: Writing \"%s\"\n", message);
    CopyMemory(pBuf, message, strlen(message) + 1);
    
    // Inicializar estructuras
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    // Crear proceso hijo
    if (!CreateProcess(
        NULL,           // No module name (use command line)
        cmdLine,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)            // Pointer to PROCESS_INFORMATION structure
    ) {
        printf("CreateProcess failed (%lu)\n", GetLastError());
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        return 1;
    }
    
    // Esperar a que el hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    // Limpiar
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return 0;
}
