#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <string>
#include <iostream>

namespace std
{
#ifdef _UNICODE
    typedef std::wstring tstring;
#define tcout std::wcout
#else
	typedef std::cout tcout;
#define tcout std::cout
#endif
}

#endif