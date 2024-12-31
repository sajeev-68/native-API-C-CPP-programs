#include<Windows.h>
#include<iostream>
#include<wchar.h>
#include<tchar.h>
#include<Psapi.h>
#define NT_SUCCESS(x) ((x)>=0)

// Define function prototype

typedef NTSTATUS(NTAPI* _NtQuerySysteminformation)(
    ULONG SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
);

// Define SYSTEM_HANDLE structure in SYSTEM_HANDLE_INFORMATION

typedef struct _SYSTEM_HANDLE{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

// Define SYSTEM_HANDLE_INFORMATION

typedef struct _SYSTEM_HANDLE_INFORMATION{
    ULONG HandleCount;
    SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

// Resole the PID obtained from FindProcAddress function
// Usually Process ID would be in DWORD form but ULONG also works.

void ResovlePid(ULONG Process_Id){

    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, Process_Id);

    if (NULL !=hProcess){
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),&cbNeeded)){
            GetModuleBaseName(hProcess, hMod, szProcessName, Process_Id);
        }
    }
_tprintf(TEXT("%s (PID: %u)\n"), szProcessName, Process_Id);
CloseHandle(hProcess);
}

// FInd the PID of the Handle

ULONG FindProcAccess(USHORT handle_no){
    DWORD pid;
    USHORT hand;
    HINSTANCE hNtDll = LoadLibraryA("ntdll.dll");
    PSYSTEM_HANDLE_INFORMATION buffer;
    ULONG buffersize = 0x10000000;
    buffer = (PSYSTEM_HANDLE_INFORMATION)malloc(buffersize);
    NTSTATUS status;

    _NtQuerySysteminformation NtQuerySysteminformation = _NtQuerySysteminformation(GetProcAddress(hNtDll,"NtQuerySystemInformation"));
    status = NtQuerySysteminformation(0x10, buffer, buffersize, NULL);

    if(!NT_SUCCESS(status)){
        std::cout << "Failed to invoke NtQuerySystemInformation! 0x" << std::hex << status << std::endl;
        return 0;
    }

    for (ULONG i = 0; i < buffer->HandleCount; i++) {
        hand = buffer->Handles[i].Handle;
        if (hand == handle_no) {
             pid= buffer->Handles[i].ProcessId;
        }
    }
    return pid;
    free(buffer);
}

int main(){
    USHORT handleno;
    ULONG procid;
    std::cout << "Handle (in decimal, convert the hex value to decimal) : ";
    std::cin >> handleno;
    procid = FindProcAccess(handleno);
    ResovlePid(procid);
    getchar();
    return 0;
}
