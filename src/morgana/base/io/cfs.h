#ifndef __MORGANA_BASE_IO_CFS_H__
#define __MORGANA_BASE_IO_CFS_H__

#include "fsa.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class CloudFileSystem : public GenericFileSystem
			{
				__declare_class(CloudFileSystem, GenericFileSystem);
			public:
				CloudFileSystem();
				virtual ~CloudFileSystem();

				String	GetDescription();
				bool	CanHandleProtocol(const char* p);
				Stream*	GetStream(const char* location);
			};
		}
	}
}

#endif