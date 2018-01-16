#ifndef __MORGANA_BASE_TYPES_TAGGED_H__
#define __MORGANA_BASE_TYPES_TAGGED_H__

#include "mearray.h"
#include "../memory/macros.h"
namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			class IsTagged
			{
			public:
				const void			SetTag(const char* tag) { if(HasTag(tag) == false) m_pTags.Add(tag); }
				const bool			HasTag(const char* tag) const
				{
					if(strstr(tag, ";") == NULL)
						return FindTag(tag) >= 0;
					else // multiple tags
					{
						String tmp(tag);
						Array<String> tags;
						tmp.Split(";", tags);
						for(String *ptr = tags.ptr(), *end = tags.end(); ptr < end; ptr++)
						{
							if(FindTag(ptr->c_str()) < 0) return false;
						}
						return true;
					}
				}
				const void			RemoveTag(const char* tag)
				{
					const int index = FindTag(tag);
					if(index >= 0)
						m_pTags.RemoveNo(index);
				}

			protected:

				Array<StringHash>	m_pTags;
				const int			FindTag(const char* tag) const
				{
					const int hash = String::ComputeHashCode(tag);
					for(StringHash *ptr = m_pTags.ptr(), *end = m_pTags.end(); ptr < end; ptr++)
						if(ptr->Equals(tag, hash))
							return ptr - m_pTags.ptr();

					return -1;
				}
			};
		}
	}
}

#endif