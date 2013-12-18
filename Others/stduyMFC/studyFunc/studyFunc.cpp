// studyFunc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tchar.h>
#include <windows.h>

void test(TCHAR** name)
{
	::LoadLibrary(*name);
}
int _tmain(int argc, _TCHAR* argv[])
{
}

