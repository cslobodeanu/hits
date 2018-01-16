#ifndef __MORGANA_FMK_SHELL_MESHELL_H__
#define __MORGANA_FMK_SHELL_MESHELL_H__

#include "reflection.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Shell
		{
			class MEShell : public MEObject
			{
				__declare_class(MEShell, MEObject);

				Array<String>	m_pLines;

				void			FormatString(const char*& fmtStr, String& strOut);
				void			PushLine(const char* str);

			public:

				MEShell();
				~MEShell();

				void DebugText(const char* str, ...);
				void DebugTextFormatted(const char*& str);
				void WarningText(const char* str, ...);
				void ErrorText(const char* str, ...);

				bool Execute(const char* str, ...);

				const Array<String>& GetLines() const { return m_pLines; }
			};
		}
	}
}

#endif