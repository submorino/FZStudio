#include "StdAfx.h"
#include "AntiHook.h"

//Ŀ���Ӧ������GameRes.dll�Ĺ���
//��ֲ��DLL���У���ӦDLL_ATTACK_PROCESS(��ѡ) �� DLL_ATTACK_THREAD(����)
//��ͬ�̷߳��䲻ͬ��TLS�洢bAntiHook��bBlockedHook����

AntiHook::AntiHook(void)
{
	//Call SetWindowsHookEx to setup my hookproc with WH_CBT
	//��װHOOK��ģ���õľ����DLLģ�������̲߳���Ϊ0��ʾȫ��
}

AntiHook::~AntiHook(void)
{
	//unhook my hookproc
}

LRESULT AntiHook::HCBTProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	//��������������ʱ��Ԥ�ƻ��������֪ͨ�������ǵȵ�������SetWindowsHookEx��֪ͨ
	//�ų����ǵ��߳�
	if(bOwnThread){
		//�ж��Ƿ��Ѿ�������߳�HOOK
		if(!bAntiHook){
			//��ȡSetWindowsHookEx�滻�ɣ�fakeSetWindowsHookEx
		}
	}

	return CallNextHookEx(hhk,nCode,wParam,lParam);
}

LRESULT AntiHook::fakeSetWindowsHookEx(int idHook,HOOKPROC lpfn,HINSTANCE hMod,DWORD dwThreadId)
{
	//ǰ������������Ѿ�������
	BlockTargetHook();
	return realHCBTProc(nCode,wParam,lParam);
}

void AntiHook::BlockTargetHook()
{
	if(bBlockedHook){
		//ͨ����ջ��������Ӧ��HCBTProc����
		//����ָ�����ȵĺ���ͷ���룬����A0
		//ȡA0��Ӧ��4���ֽڳ�ָ��B�����иߵ�λת��
		//�޸�ָ��B�ڲ�����Ϊ�档
	}
}