#ifndef __MORGANA_BASE_PARSERS_BLOCK_PARSER_H__
#define __MORGANA_BASE_PARSERS_BLOCK_PARSER_H__

#include "../types/mestring.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Types;
		namespace Parsers
		{
			class _BlockParser
			{
			public:
				void		RemoveComments(String& src);
				void		ResolveDefines(String& src);
				void		RemoveTabs(String& src);
				void		RemoveCRLF(String& src, const char replaceWith = 0);
				void		RemoveQuotes(String& src);
				void		TrimSpecialChars(String& src);
			protected:
				static		Array<String>	defines;
			};
		}
	}
}

#endif