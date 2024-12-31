#include<Windows.h>
#include<psapi.h>
#include<iostream>
#include<tchar.h>

void PrintProcessNameandID( DWORD processID ){
    
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    if (NULL != hProcess){
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded)){
            GetModuleBaseName(hProcess, hMod, szProcessName, processID );
        }
    }
_tprintf(TEXT("%s (PID: %u)\n"), szProcessName, processID);

CloseHandle(hProcess);
}

int main(void){

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)){
        return 1;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    for(i = 0; i < cProcesses; i++){
        if(aProcesses[i]!=0)
        {
            PrintProcessNameandID( aProcesses[i] );
        }
    }
    getchar();
}