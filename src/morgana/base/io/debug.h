#ifndef __MORGANA_BASE_IO_DEBUG_H__
#define __MORGANA_BASE_IO_DEBUG_H__

#include "platform.h"
#include "../compatibility/compatibility.h"

#ifdef OS_WINDOWS_WIN32
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <windows.h>
#include <WinBase.h>
#endif

#ifdef _DEBUG
#ifdef OS_WINDOWS_WIN32
#include <crtdbg.h>

#ifndef MORGANA_SHELL
#define  DEBUG_OUT(...){\
	char  _tmp[1024];\
	snprintf(_tmp, 1024,"\n"  __VA_ARGS__);\
	_tmp[1023] = '\0';\
	OutputDebugString(_tmp);\
	printf(_tmp);\
}
#else
void DEBUG_OUT(const char* str, ...);
void DEBUG_OUT();
#define MUST_IMPLEMENT_DEBUG_OUT
#endif

#define ASSERT(cond, ...)		{ if(!(cond)) { DEBUG_OUT(__VA_ARGS__);  _CrtDbgBreak(); } }

#else
#define DEBUG_OUT(...) {}
#endif
#else
#define DEBUG_OUT(...) {}
#define ASSERT(cond, ...)
#endif

#define NOT_PERMITTED		ASSERT(false, "Not permitted!");
#define NOT_IMPLEMENTED		ASSERT(false, "Not implemented!");

#endif