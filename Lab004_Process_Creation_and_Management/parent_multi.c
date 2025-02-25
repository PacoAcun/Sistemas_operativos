#include <windows.h>
#include <stdio.h>

#define NUM_CHILDREN 3

int main() {
    STARTUPINFO si[NUM_CHILDREN];
    PROCESS_INFORMATION pi[NUM_CHILDREN];
    char cmdLine[3][100];
    
    printf("Parent Process: PID=%lu\n", GetCurrentProcessId());
    
    // Crear múltiples procesos hijos
    for (int i = 0; i < NUM_CHILDREN; i++) {
        // Inicializar estructuras
        ZeroMemory(&si[i], sizeof(STARTUPINFO));
        si[i].cb = sizeof(STARTUPINFO);
        ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
        
        // Preparar línea de comandos con el número de hijo
        sprintf(cmdLine[i], "child_multi.exe %d", i + 1);
        
        // Crear proceso hijo
        if (!CreateProcess(
            NULL,                // No module name (use command line)
            cmdLine[i],          // Command line with child number
            NULL,                // Process handle not inheritable
            NULL,                // Thread handle not inheritable
            FALSE,               // Set handle inheritance to FALSE
            0,                   // No creation flags
            NULL,                // Use parent's environment block
            NULL,                // Use parent's starting directory 
            &si[i],              // Pointer to STARTUPINFO structure
            &pi[i])              // Pointer to PROCESS_INFORMATION structure
        ) {
            printf("CreateProcess failed for child %d (%lu)\n", i + 1, GetLastError());
            continue;
        }
    }
    
    // Esperar a que todos los hijos terminen
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (pi[i].hProcess != NULL) {
            WaitForSingleObject(pi[i].hProcess, INFINITE);
            CloseHandle(pi[i].hProcess);
            CloseHandle(pi[i].hThread);
        }
    }
    
    return 0;
}
