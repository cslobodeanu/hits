#ifndef __MORGANA_BASE_IO_MFS_H__
#define __MORGANA_BASE_IO_MFS_H__

#include "fsa.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class MemoryFileSystem : public GenericFileSystem
			{
				__declare_class(MemoryFileSystem, GenericFileSystem);
			public:
				MemoryFileSystem();
				virtual ~MemoryFileSystem();

				String	GetDescription();
				bool	CanHandleProtocol(const char* p);
				Stream*	GetStream(const char* location);
			};
		}
	}
}

#endif