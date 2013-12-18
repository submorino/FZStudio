#include "StdAfx.h"
#include "AntiHook.h"

//目标对应八闽游GameRes.dll的钩子
//移植到DLL库中，响应DLL_ATTACK_PROCESS(可选) 和 DLL_ATTACK_THREAD(必须)
//不同线程分配不同的TLS存储bAntiHook和bBlockedHook变量

AntiHook::AntiHook(void)
{
	//Call SetWindowsHookEx to setup my hookproc with WH_CBT
	//安装HOOK，模块用的句柄是DLL模块句柄，线程参数为0表示全局
}

AntiHook::~AntiHook(void)
{
	//unhook my hookproc
}

LRESULT AntiHook::HCBTProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	//当其它程序启动时，预计会先有这个通知，而不是等到调用完SetWindowsHookEx才通知
	//排除我们的线程
	if(bOwnThread){
		//判断是否已经对这个线程HOOK
		if(!bAntiHook){
			//获取SetWindowsHookEx替换成，fakeSetWindowsHookEx
		}
	}

	return CallNextHookEx(hhk,nCode,wParam,lParam);
}

LRESULT AntiHook::fakeSetWindowsHookEx(int idHook,HOOKPROC lpfn,HINSTANCE hMod,DWORD dwThreadId)
{
	//前提是这个钩子已经正常。
	BlockTargetHook();
	return realHCBTProc(nCode,wParam,lParam);
}

void AntiHook::BlockTargetHook()
{
	if(bBlockedHook){
		//通过堆栈分析出对应的HCBTProc函数
		//遍历指定长度的函数头代码，查找A0
		//取A0对应的4个字节长指针B，进行高低位转换
		//修改指针B内部数据为真。
	}
}