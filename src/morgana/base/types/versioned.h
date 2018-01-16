#ifndef __MORGANA_BASE_TYPES_VERSIONED_H__
#define __MORGANA_BASE_TYPES_VERSIONED_H__

#include "mearray.h"
#include "../memory/macros.h"
namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			class IsVersioned
			{
			public:
				IsVersioned() { version = 0; }

				inline const void	SetVersion(const long& v) { version = v; }
				inline const long	GetVersion() const { return version; }
				inline const bool	HasVersion(const long& other) { return version == other; }
				inline const void	IncrementVersion() { version++; }

			protected:
				long			version;
			};
		}
	}
}

#endif