#include "meshell.h"
#include <stdarg.h>

using namespace MorganaEngine::Framework::Shell;
using namespace MorganaEngine::Base::Types;

__implement_class(MEShell, MEObject);

void MShell_DebugOut_FormattedString(const char*& strFmt)
{
	singleton<MEShell>()->DebugTextFormatted(strFmt);
}

MEShell::MEShell()
{

}

MEShell::~MEShell()
{

}

void MEShell::DebugText(const char* str, ...)
{
	String strOut;
	FormatString(str, strOut);
	PushLine(strOut);
}

void MEShell::WarningText(const char* str, ...)
{
	String strOut;
	FormatString(str, strOut);
	PushLine(strOut);
}

void MEShell::ErrorText(const char* str, ...)
{
	String strOut;
	FormatString(str, strOut);
	PushLine(strOut);
}

bool MEShell::Execute(const char* str, ...)
{
	String tmp = String::Format(str);
	WarningText("Nothing to execute (%s)", tmp.c_str());
	return false;
}

void MEShell::FormatString(const char*& fmtStr, String& strOut)
{
	strOut.Format(fmtStr);
}

void MEShell::PushLine(const char* str)
{
	m_pLines.Add(str);
#ifdef OS_WINDOWS_WIN32
	OutputDebugString(str);
	OutputDebugString("\n");
#endif
	printf("%s\n", str);
}

void MEShell::DebugTextFormatted(const char*& str)
{
	String s = String::FormatF(str);
	PushLine(s);
}
