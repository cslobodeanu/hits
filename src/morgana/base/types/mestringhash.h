#ifndef __MORGANA_BASE_TYPES_STRING_HASH_H__
#define __MORGANA_BASE_TYPES_STRING_HASH_H__

#include "mestring.h"
namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			class StringHash : public String
			{
				int	mHashValue;
			protected:
				virtual void OnStringChanged();
			public:
				StringHash();
				StringHash(const char* str);
				const int GetHashValue() const;

				bool Equals(const StringHash& s) const;
				bool Equals(const char* s, const int hash = -1) const;

				const StringHash& operator = (const char* str);
				operator const char* () const;
			};
		}
	}
}

#endif