#ifndef __MORGANA_BASE_PARSERS_SIMPLE_BLOCK_PARSER_H__
#define __MORGANA_BASE_PARSERS_SIMPLE_BLOCK_PARSER_H__

#include "bp.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Parsers
		{
			class SimpleBlockParser : public _BlockParser
			{
			public:
				struct Block_t
				{
					String	name;
					Array<String> params;
					Array<String> values;

					String GetValue(const char* str)
					{
						for(int i = 0; i < params.Length(); i++)
						{
							if(params[i].EqualsNC(str))
								return values[i];
						}
						return "";
					}

					bool HasValue(const char* str)
					{
						for (int i = 0; i < params.Length(); i++)
						{
							if (params[i].EqualsNC(str))
								return true;
						}
						return false;
					}
				};

				SimpleBlockParser();
				virtual ~SimpleBlockParser();

				void		Parse(const char* src, Array<Block_t>& blocks);

				static String GetDefineValue(const char* name);

			protected:
				void		ParseBlock(String& s, Array<String>& params);
				static		int				m_DefinesContext;
			};
		}
	}
}

#endif