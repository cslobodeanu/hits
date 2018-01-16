#ifndef __MORGANA_BASE_IO_RFS_H__
#define __MORGANA_BASE_IO_RFS_H__

#include "fsa.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class RegistryFileSystem : public GenericFileSystem
			{
				__declare_class(RegistryFileSystem, GenericFileSystem);
				bool isInited;
				void Initialize();
				void ReadFromDisk();
				void WriteToDisk();

				struct RegistryEntry_t
				{
					StringHash		name;
					Stream*			data;

					RegistryEntry_t()
					{
						data = NULL;
					}

					RegistryEntry_t(const RegistryEntry_t& other)
					{
						name = other.name;
						data = other.data;
					}
				};

				Array<RegistryEntry_t> entries;

				Stream*	GetFile();

				bool			isDiskOperation;

			protected:
				virtual void	OnStreamRetrieved(Stream* s);
				virtual void	OnStreamClosed(Stream* s);
			public:
				RegistryFileSystem();
				virtual ~RegistryFileSystem();

				String	GetDescription();
				bool	CanHandleProtocol(const char* p);
				Stream*	GetStream(const char* location);
			};
		}
	}
}

#endif