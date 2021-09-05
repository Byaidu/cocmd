// 加载 CMD 实例并初始化钩子函数

#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include <pthread.h>
#include <hook.h>
#include <CAPIx.h>
#include <lock.h>

using namespace std;

typedef int (*func1)();

BYTE Data[4096];
HKEY hKey;
HMODULE hdll;
DWORD dllcnt = 0;
DWORD dllloadcnt = 1;
WCHAR dllpath[4096];
WCHAR dllname[4096];
pthread_t threadlist[1024];
int argc;
WCHAR **argv;

pthread_mutex_t mutex_load = PTHREAD_MUTEX_INITIALIZER;

LONG
APIENTRY
RegOpenKeyExW_Hook(
    HKEY hKey,
    LPCWSTR lpSubKey,
    DWORD ulOptions,
    REGSAM samDesired,
    PHKEY phkResult
    )
{
    // 禁用 HKLM 查询，避免重复调用 RegQueryValueExW
    if (hKey == HKEY_LOCAL_MACHINE){
        return 1;
    }else{
        return RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    }
}

LONG
APIENTRY
RegQueryValueExW_Hook(
    HKEY hKey,
    LPCWSTR lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData
    )
{
    // 通过 AutoRun 注入 CMD 指令
    if (!wcscmp(lpValueName,L"AutoRun")){
        pthread_mutex_lock(&mutex_load);
        //printf("%ls %d\n",argv[1],dllloadcnt);
        wcscpy((PWCHAR)lpData, argv[dllloadcnt]);
        dllloadcnt++;
        pthread_mutex_unlock(&mutex_load);
        *lpType = REG_EXPAND_SZ;
        *lpcbData = (wcslen((PWCHAR)lpData) + 1) * 2;
        return 0;
    }else{
        return RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
    }
}

int main() {
    argv=CommandLineToArgvW(GetCommandLineW(), &argc);

    for (int i=1;i<argc;i++){
        //复制 CMD 实例
        GetTempPathW(4096, dllpath);
        swprintf(dllname,L"%scmd_%d.dll",dllpath,dllcnt);
        CopyFileW(L"cmd.dll",dllname,false);
        hdll = LoadLibraryW(dllname);
        //printf("ERROR:%d\n",GetLastError());

        // 安装 CAPI 钩子
        bakSetEnv_capi = (bool *)HookAPI(hdll, "SetEnvironmentVariableW", (PVOID)SetCall_CAPI);

        // 安装 LOCK 钩子
        bakSetEnv_lock = (bool *)HookAPI(hdll, "SetEnvironmentVariableW", (PVOID)SetCall_LOCK);

        //通过安装注册表钩子注入 CMD 指令
        HookAPI(hdll, "RegQueryValueExW", (PVOID)RegQueryValueExW_Hook);
        HookAPI(hdll, "RegOpenKeyExW", (PVOID)RegOpenKeyExW_Hook);

        //启动目标线程
        pthread_create(&threadlist[i],NULL,(void*(*)(void*))((unsigned char*)hdll + 0x829A),NULL);
        dllcnt++;
    }

    while (1){Sleep(1000);}

    return 0;
}
