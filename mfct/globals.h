#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <string>
#include <iostream>

namespace std
{
#ifdef _UNICODE
    typedef wstring tstring;
#define tcout wcout
#else
	typedef string tstring;
#define tcout cout
#endif
}

extern HMODULE GetCurrentModule(void);


#endif