#include <Windows.h>
#include <string>
#include <tchar.h>
#include <iostream>
#include <Psapi.h>

int FindHandleName(const std::string& handle_str) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    DWORD aProcesses[1024], cbNeeded, cProcesses, processID;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return 1;
    }
    
    cProcesses = cbNeeded / sizeof(DWORD);

    // Convert handle_str from hex string to HANDLE (if needed)
    HANDLE targetHandle = reinterpret_cast<HANDLE>(std::stoul(handle_str, nullptr, 16));

    for (DWORD i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            processID = aProcesses[i];
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

            if (hProcess == NULL) {
                continue;
            }

            // Optionally compare hProcess with targetHandle here
            if (hProcess == targetHandle) {
                HMODULE hMod;
                DWORD cbModNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbModNeeded)) {
                    GetModuleBaseName(hProcess, hMod, szProcessName, MAX_PATH);
                    _tprintf(TEXT("Process Name: %s (PID: %u)\n"), szProcessName, processID);
                }
                CloseHandle(hProcess);
                return 0; // Found the handle, exiting function
            }

            CloseHandle(hProcess);
        }
    }

    std::cout << "Handle not found or process inaccessible.\n";
    return 1;
}

int main() {
    std::string handle_hex;
    std::cout << "Enter the Handle (in hex format): ";
    std::cin >> handle_hex;

    FindHandleName(handle_hex);

    getchar();
    return 0;
}
