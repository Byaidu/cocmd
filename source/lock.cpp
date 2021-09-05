// 实现线程锁
// set thread=lock
// set thread=unlock

#include <pthread.h>
#include <windows.h>

#define SetEnvW ((PFNSETENVIRONMENTVARIABLE)bakSetEnv_lock)

typedef bool  (WINAPI *PFNSETENVIRONMENTVARIABLE)(wchar_t *, wchar_t *);

bool *bakSetEnv_lock;

pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

bool WINAPI SetCall_LOCK(wchar_t *varName, wchar_t *varValue)
{
    //printf("SetCall_CAPI:%ls %ls\n",varName ,varValue);
    if (!wcsicmp(varName, L"thread")) {
        if (!wcsicmp(varValue, L"lock")){
            pthread_mutex_lock(&mutex_lock);
        }
        if (!wcsicmp(varValue, L"unlock")){
            pthread_mutex_unlock(&mutex_lock);
        }
    } else {
        SetEnvW(varName, varValue);
    }
    return true;
}