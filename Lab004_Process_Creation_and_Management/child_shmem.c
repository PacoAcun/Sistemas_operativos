// child_shmem.c
#include <windows.h>
#include <stdio.h>

#define SHARED_MEM_SIZE 1024
#define SHARED_MEM_NAME "Local\\SharedMemoryExample"

int main() {
    HANDLE hMapFile;
    LPVOID pBuf;
    
    // Abrir objeto de mapeo de archivo existente
    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,     // Permiso de lectura/escritura
        FALSE,                   // No heredable
        SHARED_MEM_NAME);        // Nombre del objeto de mapeo
    
    if (hMapFile == NULL) {
        printf("OpenFileMapping failed (%lu)\n", GetLastError());
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
    
    // Leer mensaje de memoria compartida
    printf("Child Process: Read \"%s\"\n", (LPTSTR)pBuf);
    
    // Limpiar
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    
    return 0;
}