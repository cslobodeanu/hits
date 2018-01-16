#include "debug.h"

#ifdef MUST_IMPLEMENT_DEBUG_OUT

extern void MShell_DebugOut_FormattedString(const char*& strFmt);
void DEBUG_OUT(const char* str, ...)
{
	MShell_DebugOut_FormattedString(str);
};

void DEBUG_OUT()
{

}

#endif