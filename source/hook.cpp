// 根据函数名安装钩子函数，支持重复安装

#include <windows.h>
#include <cstdio>

PVOID HookAPI(HMODULE hMod, PCHAR pOldFunc, PVOID pNewFunc)
{
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_NT_HEADERS pNTHeaders;
    PIMAGE_OPTIONAL_HEADER    pOptHeader;
    PIMAGE_IMPORT_DESCRIPTOR  pImportDescriptor;
    PIMAGE_THUNK_DATA         pINT,pIAT;
    PIMAGE_IMPORT_BY_NAME     pImportByName;
    PCHAR   pDllName,pFuncName;
    PVOID  pFunction;

    pDosHeader = (PIMAGE_DOS_HEADER)hMod;
    pNTHeaders = (PIMAGE_NT_HEADERS)((BYTE *)hMod + pDosHeader->e_lfanew);
    pOptHeader = (PIMAGE_OPTIONAL_HEADER)&(pNTHeaders->OptionalHeader);

    pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE *)hMod + pOptHeader->DataDirectory[1].VirtualAddress);

    while(pImportDescriptor->FirstThunk)
    {
        //DLL Name
        pDllName = (PCHAR)((BYTE *)hMod + pImportDescriptor->Name);
        // printf("%s\n",pDllName);

        // INT & IAT
        pINT = (PIMAGE_THUNK_DATA)((BYTE *)hMod + pImportDescriptor->OriginalFirstThunk);
        pIAT = (PIMAGE_THUNK_DATA)((BYTE *)hMod + pImportDescriptor->FirstThunk);

        while(pINT->u1.AddressOfData)
        {
            //FUNC NAME
            pImportByName = (PIMAGE_IMPORT_BY_NAME)((BYTE *)hMod + pINT->u1.AddressOfData);
            pFuncName = (PCHAR)pImportByName->Name;
            pFunction = (PVOID)pIAT->u1.Function;

            //printf("%s %08x\n", pFuncName, pFunction);
            if (!strcmp(pFuncName, pOldFunc)){
                //printf("%s %s:%08x\n", pDllName, pFuncName, pFunction);
                DWORD dwOLD;
                MEMORY_BASIC_INFORMATION mbi;
                VirtualQuery(pIAT,&mbi,sizeof(mbi));
                VirtualProtect(pIAT,sizeof(DWORD),PAGE_READWRITE,&dwOLD);

                WriteProcessMemory(GetCurrentProcess(),
                pIAT, &pNewFunc, sizeof(DWORD), NULL);

                VirtualProtect(pIAT,sizeof(DWORD),dwOLD,0);

                return pFunction;
            }

            pINT++;
            pIAT++;
        }
        pImportDescriptor++;
    }
    return 0;
}