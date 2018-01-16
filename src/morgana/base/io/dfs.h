#ifndef __MORGANA_BASE_IO_DFS_H__
#define __MORGANA_BASE_IO_DFS_H__

#include "fsa.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class DiskFileSystem : public GenericFileSystem
			{
				__declare_class(DiskFileSystem, GenericFileSystem);
			public:
				DiskFileSystem();
				virtual ~DiskFileSystem();

				String	GetDescription();
				bool	CanHandleProtocol(const char* p);
				Stream*	GetStream(const char* location);
			};

			class FileStream : public Stream
			{
			private:
				int32	size;
				FILE*	file;
			public:

				FileStream();
				virtual ~FileStream();

				bool	Open(OpenMode mode);
				void	Close();

				int		ReadBuffer(void   *dest, int count);
				int		WriteBuffer(const void *src, int count);

				bool	Seek(SeekPos pos, int32 count);
				uint32	Pos();
				int32	Size();
			};
		}
	}
}

#endif