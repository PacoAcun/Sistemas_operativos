#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 256

int main() {
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    TCHAR cmdLine[] = TEXT("child_pipe.exe");
    char message[] = "Hello from Parent";
    DWORD bytesWritten;
    
    // Configurar atributos de seguridad para que los handles sean heredables
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    // Crear la tubería
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        printf("CreatePipe failed (%lu)\n", GetLastError());
        return 1;
    }
    
    // Configurar los handles para el proceso hijo
    // El hijo solo necesita el handle de lectura, así que hacemos el de escritura no heredable
    SetHandleInformation(hWritePipe, HANDLE_FLAG_INHERIT, 0);
    
    // Inicializar estructuras
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    // Convertir el handle a cadena para pasarlo como argumento
    char args[100];
    sprintf(args, "child_pipe.exe %p", hReadPipe);
    
    // Crear proceso hijo
    if (!CreateProcess(
        NULL,           // No module name (use command line)
        args,           // Command line with pipe handle
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,           // Set handle inheritance to TRUE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)            // Pointer to PROCESS_INFORMATION structure
    ) {
        printf("CreateProcess failed (%lu)\n", GetLastError());
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return 1;
    }
    
    // El padre envía un mensaje al hijo
    printf("Parent Process: Writing \"%s\"\n", message);
    if (!WriteFile(hWritePipe, message, (DWORD)strlen(message) + 1, &bytesWritten, NULL)) {
        printf("WriteFile failed (%lu)\n", GetLastError());
    }
    
    // Cerrar los handles
    CloseHandle(hWritePipe);
    
    // Esperar a que el hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hReadPipe);
    
    return 0;
}
