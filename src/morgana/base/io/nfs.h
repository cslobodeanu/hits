#ifndef __MORGANA_BASE_IO_NFS_H__
#define __MORGANA_BASE_IO_NFS_H__

#include "fsa.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class NetworkFileSystem : public GenericFileSystem
			{
				__declare_class(NetworkFileSystem, GenericFileSystem);
			public:
				NetworkFileSystem();
				virtual ~NetworkFileSystem();

				String	GetDescription();
				bool	CanHandleProtocol(const char* p);
				Stream*	GetStream(const char* location);
			};
		}
	}
}

#endif