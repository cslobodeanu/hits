#ifndef __MORGANA_BASE_UTILS_CODE_BLOCK_PARSER_H__
#define __MORGANA_BASE_UTILS_CODE_BLOCK_PARSER_H__

#include "bp.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Types;
		namespace Parsers
		{
			class CodeBlockParser : public _BlockParser
			{
			public:
				struct Block_t
				{
					String	name;
					String	contents;

					Array<Block_t> subBlocks;
				};

				void Parse(const char* src, Array<Block_t>& blocks);

				static void PrintSource(const char* src);
			};
		}
	}
}

#endif