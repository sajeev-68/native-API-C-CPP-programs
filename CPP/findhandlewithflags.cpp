#include<Windows.h>
#include<winternl.h>
#include<iostream>
#include<vector>
#include<ntstatus.h>

#pragma comment(lib, "ntdll.lib")

typedef NTSTATUS(NTAPI* _NtQuerySystemInformation)(
	ULONG SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength    
);

void GetProcessHandleAccessFlags() {
    ULONG bufferSize = 0x10000;
    PVOID buffer = nullptr;
    ULONG returnLength = 0;
    NTSTATUS status;

    buffer = malloc(bufferSize);
    if(!buffer) {
        std::cerr << "Failed to allocate memory!!" << std::endl;
        return;
    }

    status = NtQuerySystemInformation(SystemHandleInformation, buffer, bufferSize, &returnLength);


     if (status == STATUS_INFO_LENGTH_MISMATCH) {
        free(buffer);
        bufferSize = returnLength;
        buffer = malloc(bufferSize);
        if (!buffer) {
            std::cerr << "Failed to re-allocate memory for handle information." << std::endl;
            return;
     }
    status = NtQuerySystemInformation(SystemHandleInformation, buffer, bufferSize, &returnLength);
     }

     if (status != STATUS_SUCCESS) {
        std::cerr << "Failed to query system handle information." << std::endl;
        free(buffer);
        return;
    }

    // Cast the buffer to SYSTEM_HANDLE_INFORMATION
    PSYSTEM_HANDLE_INFORMATION handleInfo = (PSYSTEM_HANDLE_INFORMATION)buffer;

    // Loop through each handle and print information
    for (ULONG i = 0; i < handleInfo->Count; i++) {
        SYSTEM_HANDLE_ENTRY sysHandle = handleInfo->Handle[i];

        // Print out the process ID, handle, and granted access flags
        std::cout << "Process ID: " << sysHandle.OwnerPid << ", Handle: 0x" << std::hex << sysHandle.HandleFlags << ", Granted Access: 0x" << std::hex << sysHandle.AccessMask << std::dec << std::endl;
    }
    // Free the allocated buffer
    free(buffer);
}

int main() {
    GetProcessHandleAccessFlags();
    getchar();
    return 0;  
}