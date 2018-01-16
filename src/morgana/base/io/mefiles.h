#ifndef __MORGANA_BASE_IO_MEFILES_H__
#define __MORGANA_BASE_IO_MEFILES_H__

#include "metypes.h"
#include "reflection.h"
#include "meclock.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class Stream;
			class GenericFileSystem;
			class MFiles
			{
				Array<GenericFileSystem*>	fileSystems;
				Array<Stream*>				streams;
				GenericFileSystem*			GetProperFileSystemForLocation(const char* location);
			public:
				MFiles();
				virtual ~MFiles();

				Stream*		GetStream(const char* location);
				void		CopyStream(const char* src, const char* dst);

				void		OnStreamDeleted(Stream* s);
				void		OnStreamClosed(Stream* s);

				void		Cleanup();

				DateTime_t	GetFileTimeStamp(const char* location);
			};
		}
	}
}

#endif