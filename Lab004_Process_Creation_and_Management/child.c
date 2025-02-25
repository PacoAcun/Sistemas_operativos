// child.c - Proceso hijo que imprime su PID y el PID de su padre
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

// Función para obtener el PID del proceso padre
DWORD GetParentProcessId() {
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    DWORD ppid = 0;
    DWORD pid = GetCurrentProcessId();
    
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }
    
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return 0;
    }
    
    do {
        if (pe32.th32ProcessID == pid) {
            ppid = pe32.th32ParentProcessID;
            break;
        }
    } while (Process32Next(hSnapshot, &pe32));
    
    CloseHandle(hSnapshot);
    return ppid;
}

int main() {
    DWORD pid = GetCurrentProcessId();
    DWORD ppid = GetParentProcessId();
    
    printf("Child Process: PID=%lu, Parent PID=%lu\n", pid, ppid);
    
    return 0;
}