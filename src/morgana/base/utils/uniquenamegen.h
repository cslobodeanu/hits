#ifndef __MORGANA_BASE_UTILS_UNIQUE_NAME_GENERATOR_H__
#define __MORGANA_BASE_UTILS_UNIQUE_NAME_GENERATOR_H__

#include "../types/mestring.h"
namespace MorganaEngine
{
	namespace Base
	{
		namespace Utils
		{
			class UniqueNameGenerator
			{
			public:
				static Types::String Generate(const char* base)
				{
					static unsigned int _count = 1;
					Types::String s;
					s.Printf("%s%03d", base, _count);
					_count++;
					return s;
				}

				static Types::String ConvertVarNameToDisplayName(const char* base)
				{
					String s = "";

					for (int i = 0; base[i] != 0; i++)
					{
						char c = base[i];
						if (i == 0)
						{
							c = String::ToUpperCase(c);
						}
						else
							if (String::IsUpperCase(c))
							{
								s += " ";
							}

						s += c;
					}
					
					return s;
				}
			};
		}
	}
}

#endif