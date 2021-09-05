extern bool *bakSetEnv_capi;
extern DWORD *bakGetEnv_capi;

bool WINAPI SetCall_CAPI(wchar_t *varName, wchar_t *varValue);
DWORD WINAPI GetCall_CAPI(wchar_t *varName, wchar_t* varValue, DWORD size);