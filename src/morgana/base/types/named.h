#ifndef __MORGANA_BASE_TYPES_NAMED_H__
#define __MORGANA_BASE_TYPES_NAMED_H__

#include "mearray.h"
#include "../memory/macros.h"
#include "mestringhash.h"
#include "../reflection/callbacks.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Reflection;
		namespace Types
		{
			class IsNamed
			{
			public:
				IsNamed() { name = "(unnamed)"; version = 0; }

				Action<>				OnNameChanged;
				int						version;

				inline const void		SetName(const char* name) { if (name != NULL) this->name = name; else this->name = ""; OnNameChanged(); version++; }
				inline const String		GetName() const { return name; }
				inline const String&	GetNameRef() const { return name; }
				inline bool				HasName(const char* name, const int hash = -1)
				{
					return this->name.Equals(name, hash);
				}

			protected:
				StringHash			name;
			};
		}
	}
}

#endif