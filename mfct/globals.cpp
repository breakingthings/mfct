#include "stdafx.h"

HMODULE GetCurrentModule(void)
{
	HMODULE hModule = NULL;
	GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetCurrentModule,
		&hModule);
	return hModule;
}
