
#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    TCHAR cmdLine[] = TEXT("child.exe");
    
    // Inicializar estructuras
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    printf("Parent Process: PID=%lu\n", GetCurrentProcessId());
    
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
        return 1;
    }
    
    // Dar tiempo al proceso hijo para imprimir su mensaje
    Sleep(1000);
    
    // Cerrar los handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return 0;
}
