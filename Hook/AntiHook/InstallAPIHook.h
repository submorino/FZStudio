
typedef BOOL (WINAPI *SetWindowsHookExANextHook_t)(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId);
SetWindowsHookExANextHook_t SetWindowsHookExANextHook = NULL;
typedef BOOL (WINAPI *SetWindowsHookExWNextHook_t)(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId);
SetWindowsHookExWNextHook_t SetWindowsHookExWNextHook = NULL; 

static void InstallAPIHooks();

static void UninstallAPIHooks();