#define BUILDING_DLL

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <malloc.h>

#include "HookImportFunction.h"
#include "AntiHook.h"

#define JMPSIZE	20
#define MEMSIZE	4096

//Ŀ���Ӧ������GameRes.dll�Ĺ���
//��ֲ��DLL���У���ӦDLL_ATTACK_PROCESS(��ѡ) �� DLL_ATTACK_THREAD(����)
//��ͬ�̷߳��䲻ͬ��TLS�洢bAntiHook��bBlockedHook����

CRITICAL_SECTION cs;

HINSTANCE g_hInstance = NULL;
HHOOK g_hHookCBT = NULL;
HANDLE hSynhroMutex = 0;

//���������ڴ棬����������ڡ�
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
	/*char JMPCODE[JMPSIZE];//Ŀǰ��5���ֽڣ���װ��C++��չ�������ֽڲ�����
	char OldCodeCache[JMPSIZE];*/
	CodeCache* Old;
	PROC* HookFuc;
}HOOK_STATUS,*PHOOK_STATUS;

//���Ӻ�����ַ
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
	

	//��ָ֤�������ԣ���Ҫ�����һ�µ�ǰ��ַ�Ĵ��룬Ȼ��ʵ�ֽ�ȡ������
	//���ڵ�ַ������Ҫȡ��ַ��ת�ֽ�Ȼ�����������ࡣ
	
	//1.ʹ�û�������ɻ����룬Ȼ���Ƶ�IndirectHook�н���ʵ��
	//2.�����������Ҫ�滻�Ĵ��룬�滻HOOKĿ���ַͷ��ַ��call��IndirectHook

	/*
	PHOOK_STATUS param = (PHOOK_STATUS)lpvBase;
	unsigned int size = sizeof(param->JMPCODE)/sizeof(param->JMPCODE[0]);
	//��ַ����ƫ����
	int offset =((int)(int*)((PROC*)(newFuc)))-((int)(int*)(PROC*)oldFuc) - JMPSIZE;
	memcpy((param->JMPCODE)+1,&offset,sizeof(newFuc));
	memcpy(param->OldCodeCache,(PROC*)oldFuc,size);
	memcpy((PROC*)oldFuc,(param->JMPCODE),size);
	param->HookFuc = oldFuc;
	*/
}

void UnInlineHook()
{
	//1.�ָ�HOOKĿ�걻�滻���ֽڴ���
	//2.���IndirectHook��������ʼ��Ϊ0x90
	PHOOK_STATUS param = (PHOOK_STATUS)lpvBase;
	unsigned int size = sizeof(param->OldCodeCache)/sizeof(param->OldCodeCache[0]);
	memcpy((PROC*)param->HookFuc,param->OldCodeCache,size);
}

LRESULT CALLBACK HCBTProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	//��������������ʱ��Ԥ�ƻ��������֪ͨ�������ǵȵ�������SetWindowsHookEx��֪ͨ
	//�ų����ǵ��߳�
	//DWORD curThreadId = ::GetCurrentThreadId();
	//if(bOwnThread){
	//�ж��Ƿ��Ѿ�������߳�HOOK
	//if(!bAntiHook){
	//Interexchange
	//��ȡSetWindowsHookEx�滻�ɣ�fakeSetWindowsHookEx
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
	////ǰ������������Ѿ�������
	////BlockTargetHook();

	//OutputDebugStringA("fakeSetWindowsHookEx Excutation");

	//// JMP��ԭ���ĺ�������
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
	//ͨ����ջ��������Ӧ��HCBTProc����
	//����ָ�����ȵĺ���ͷ���룬����A0
	//ȡA0��Ӧ��4���ֽڳ�ָ��B�����иߵ�λת��
	//�޸�ָ��B�ڲ�����Ϊ�档
	//}
}

DLLIMPORT void ActivateAntiHook(int activate)
{
	if (activate)
	{
		if (g_hHookCBT != NULL)	return;

		//test
		//HCBTProc(0,0,0);
		//��װHOOK��ģ���õľ����DLLģ�������̲߳���Ϊ0��ʾȫ��
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
		//����Ŀ�걻�滻�Ĵ���
		lpvBase = VirtualAlloc(NULL, sizeof(HOOK_STATUS), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (lpvBase){
			//��ע������ȡ
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
