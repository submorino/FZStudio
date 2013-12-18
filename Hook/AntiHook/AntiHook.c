#define BUILDING_DLL

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <malloc.h>

#include "HookImportFunction.h"
#include "AntiHook.h"

#define JMPSIZE	20
#define MEMSIZE	4096

//目标对应八闽游GameRes.dll的钩子
//移植到DLL库中，响应DLL_ATTACK_PROCESS(可选) 和 DLL_ATTACK_THREAD(必须)
//不同线程分配不同的TLS存储bAntiHook和bBlockedHook变量

CRITICAL_SECTION cs;

HINSTANCE g_hInstance = NULL;
HHOOK g_hHookCBT = NULL;
HANDLE hSynhroMutex = 0;

//分配虚拟内存，在这个进程内。
LPVOID lpvBase = NULL;
PVOID g_pvOffset = NULL;
PVOID g_pvOffsetAddr = NULL;


typedef HHOOK (*PfnSetWindowsHookEx)(int idHook,
									 HOOKPROC lpfn,
									 HINSTANCE hMod,
									 DWORD dwThreadId
									 );

typedef struct CodeCache{
	char* ByteCode;
	size_t SizeCode;
}

typedef struct _HOOK_STATUS{
	BOOL bAntiHook;
	HHOOK iHook;
	/*char JMPCODE[JMPSIZE];//目前是5个字节，封装成C++扩展成任意字节参数。
	char OldCodeCache[JMPSIZE];*/
	CodeCache* Old;
	PROC* HookFuc;
}HOOK_STATUS,*PHOOK_STATUS;

//钩子函数地址
PfnSetWindowsHookEx g_pHookedFun = NULL;


LRESULT fakeSetWindowsHookEx(int idHook,HOOKPROC lpfn,HINSTANCE hMod,DWORD dwThreadId);

int byte2Int(void *p)
{
	int pa = (int)(*((int*)p));
	int i = 0;
	__asm{
		mov eax,dword ptr [pa]
		mov ebx,eax
			rol bx,8
			shl ebx,16
			shr eax,16
			rol ax,8
			xor eax,ebx
			mov i,eax
	}

	return i;
}

void __declspec(naked) IndirectHook()
{
	__asm
	{
			_emit 0x90	//1
			_emit 0x90	//2
			_emit 0x90	//3
			_emit 0x90	//4
			_emit 0x90	//5
			_emit 0x90	//6
			_emit 0x90	//7
			_emit 0x90	//8
			_emit 0x90	//9
			_emit 0x90	//A
			_emit 0x90	//B
			_emit 0x90	//C
			_emit 0x90	//D
			_emit 0x90	//E
			_emit 0x90	//F

			_emit 0x90	//1
			_emit 0x90	//2
			_emit 0x90	//3
			_emit 0x90	//4
			_emit 0x90	//5
			_emit 0x90	//6
			_emit 0x90	//7
			_emit 0x90	//8
			_emit 0x90	//9
			_emit 0x90	//A
			_emit 0x90	//B
			_emit 0x90	//C
			_emit 0x90	//D
			_emit 0x90	//E
			_emit 0x90	//F

			_emit 0x90	//1
			_emit 0x90	//2
			_emit 0x90	//3
			_emit 0x90	//4
			_emit 0x90	//5
			_emit 0x90	//6
			_emit 0x90	//7
			_emit 0x90	//8
			_emit 0x90	//9
			_emit 0x90	//A
			_emit 0x90	//B
			_emit 0x90	//C
			_emit 0x90	//D
			_emit 0x90	//E
			_emit 0x90	//F

	}
}

void Assemble(__in char *pasm,__in ulong ip,__out t_asmmodel am)
{
	char buf;
	char errtext[TEXTLEN];

	//pasm ="ADD [DWORD 475AE0],1";
	sprintf(buf,"%s:\n",pasm);
	OutputDebugStringA(buf);
	if(0<Assemble(pasm,ulong ip,&am,0,0,errtext)){
		assert(0);
		OutputDebugStringA(errtext);
	}
}

//void CodeInject(__in void* InjectAddr,__in void* InjectFuc)
//{
//	char* pasm;
//	OpCode = "JMP [DWORD %s]";
//	sprintf(OpCode,format,newFuc);
//	Assemble(pasm,InjectAddr,&InjectFuc);
//}

BOOL InlineHook(void* newFuc,void* oldFuc)
{
	char* pasm;
	void* p;
	t_asmmodel am;
	PHOOK_STATUS param;

	//assemble code
	format = "JMP [DWORD %s]";
	sprintf(pasm,format,newFuc);
	Assemble(pasm,oldFuc,&am);

	//disasm inject address code byte in order to running normally

	param = (PHOOK_STATUS)lpvBase;
	p = malloc(am.length);
	memset(p,0x90,am.length);

	param->Old->SizeCode = am.length;
	param->Hook = oldFuc;

	//keey cache code and replace inject address code
	memcpy(param->Old->ByteCode,m.code,am.length);
	memcpy(param->Old->ByteCode,(PROC*)oldFuc,size);
	

	//保证指令完整性，需要反汇编一下当前地址的代码，然后实现截取操作。
	//对于地址方面需要取地址，转字节然后放入汇编器汇编。
	
	//1.使用汇编器生成汇编代码，然后复制到IndirectHook中进行实现
	//2.汇编器生成需要替换的代码，替换HOOK目标地址头地址，call到IndirectHook

	/*
	PHOOK_STATUS param = (PHOOK_STATUS)lpvBase;
	unsigned int size = sizeof(param->JMPCODE)/sizeof(param->JMPCODE[0]);
	//地址计算偏移量
	int offset =((int)(int*)((PROC*)(newFuc)))-((int)(int*)(PROC*)oldFuc) - JMPSIZE;
	memcpy((param->JMPCODE)+1,&offset,sizeof(newFuc));
	memcpy(param->OldCodeCache,(PROC*)oldFuc,size);
	memcpy((PROC*)oldFuc,(param->JMPCODE),size);
	param->HookFuc = oldFuc;
	*/
}

void UnInlineHook()
{
	//1.恢复HOOK目标被替换的字节代码
	//2.清空IndirectHook函数，初始化为0x90
	PHOOK_STATUS param = (PHOOK_STATUS)lpvBase;
	unsigned int size = sizeof(param->OldCodeCache)/sizeof(param->OldCodeCache[0]);
	memcpy((PROC*)param->HookFuc,param->OldCodeCache,size);
}

LRESULT CALLBACK HCBTProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	//当其它程序启动时，预计会先有这个通知，而不是等到调用完SetWindowsHookEx才通知
	//排除我们的线程
	//DWORD curThreadId = ::GetCurrentThreadId();
	//if(bOwnThread){
	//判断是否已经对这个线程HOOK
	//if(!bAntiHook){
	//Interexchange
	//获取SetWindowsHookEx替换成，fakeSetWindowsHookEx
	//}
	//}
	//OutputDebugStringA("HCBTProc!\n");
	__try{

		PHOOK_STATUS param = (PHOOK_STATUS)lpvBase;
		BOOL bHook = param->bAntiHook;
		
		if (bHook) 
		{
			char Buffer[256] = {0};
			DWORD OldProtect = 0;
			int offset = 0,pa = 0; 
			void * p = NULL;

			HMODULE module = LoadLibraryA("user32.dll");
			PfnSetWindowsHookEx pfn = (PfnSetWindowsHookEx)GetProcAddress(module,"SetWindowsHookExW");

			if (!VirtualProtect(pfn, JMPSIZE, PAGE_EXECUTE_READWRITE, &OldProtect))
				return 0;

			sprintf(Buffer,"SetWindowsHookEx Address:%x\n",pfn);
			OutputDebugStringA(Buffer);

			sprintf(Buffer,"jmpcode content:%x\n",param->JMPCODE);
			OutputDebugStringA(Buffer);

			//offset =((int)(int*)((PROC*)(&fakeSetWindowsHookEx)))-((int)(int*)(PROC*)pfn) - JMPSIZE;
			EnterCriticalSection(&cs);
			if(InlineHook(&fakeSetWindowsHookEx,pfn))
				InterlockedIncrement(&(param->bAntiHook));
			LeaveCriticalSection(&cs);
		}
	}__except(1)
	{
		LeaveCriticalSection(&cs);
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT fakeSetWindowsHookEx(int idHook,HOOKPROC lpfn,HINSTANCE hMod,DWORD dwThreadId)
{
	//PHOOK_STATUS p;
	//int address;

	//p = (PHOOK_STATUS)lpvBase;
	////前提是这个钩子已经正常。
	////BlockTargetHook();

	//OutputDebugStringA("fakeSetWindowsHookEx Excutation");

	//// JMP回原来的函数调用
	//
 //   address = (void*)(p->HookFuc);
	////int realcall = ((int)(int*)address)+JMPSIZE;
	////__asm jmp realcall
	//__asm retn

	return 1;
}

static void BlockTargetHook()
{
	//if(bBlockedHook){
	//通过堆栈分析出对应的HCBTProc函数
	//遍历指定长度的函数头代码，查找A0
	//取A0对应的4个字节长指针B，进行高低位转换
	//修改指针B内部数据为真。
	//}
}

DLLIMPORT void ActivateAntiHook(int activate)
{
	if (activate)
	{
		if (g_hHookCBT != NULL)	return;

		//test
		//HCBTProc(0,0,0);
		//安装HOOK，模块用的句柄是DLL模块句柄，线程参数为0表示全局
		g_hHookCBT = SetWindowsHookEx(WH_CBT, HCBTProc, GetModuleHandle(TEXT("AntiHook.dll")), 0);
	}
	else
	{
		if (g_hHookCBT == NULL)	return;

		UnhookWindowsHookEx(g_hHookCBT);
	}
}

void dllinit(HINSTANCE hInst,DWORD reason,LPVOID reserved)
{
	if(DLL_PROCESS_ATTACH == reason){
		//保存目标被替换的代码
		lpvBase = VirtualAlloc(NULL, sizeof(HOOK_STATUS), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (lpvBase){
			//从注册表里获取
			HOOK_STATUS hook = {FALSE,NULL,{0xE9,0x90,0x90,0x90,0x90},NULL};
			memcpy(lpvBase,&hook,sizeof(hook));
			InitializeCriticalSection(&cs);
		}
	}
	else if(DLL_PROCESS_DETACH == reason){
		//unhook
		UnInlineHook();
		VirtualFree(lpvBase,sizeof(HOOK_STATUS),MEM_RELEASE);
	}
}

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
					   DWORD reason        /* Reason this function is being called. */ ,
					   LPVOID reserved     /* Not used. */ )
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		dllinit(hInst,reason,reserved);
		break;

	case DLL_PROCESS_DETACH:
		dllinit(hInst,reason,reserved);
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;
	}

	/* Returns TRUE on success, FALSE on failure */
	return TRUE;
}
