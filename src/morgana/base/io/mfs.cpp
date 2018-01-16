#include "mfs.h"
#include "debug.h"
#include "mememory.h"
#include "paths.h"

using namespace MorganaEngine::Base::IO;

__implement_class(MemoryFileSystem, GenericFileSystem);

class MemStream : public Stream
{
public:
	unsigned char*	dataPtr;
	size_t			fileSize;
	size_t			filePos;
	bool			isReadOnly, m_bOwnBuffer;
	void			IncreaseSize(const int& count)
	{
		// If a MemStream was constructed with a specified buffer, don't allow the IncreaseSize on that stream.
		// The reason is that the data will not be written in the buffer provided at construction time and this might lead to some serious issues

		ASSERT(m_bOwnBuffer, "Cannot increase the size for a MemStream with the buffer provided externally! Please check your code!");

		const int new_size = fileSize + count;

		if(fileSize > 0)
		{
			unsigned char* bak = NEW unsigned char[new_size];
			memcpy(bak, dataPtr, fileSize);

			if (m_bOwnBuffer && dataPtr)
				delete [] dataPtr;

			m_bOwnBuffer = true;
			dataPtr = bak;
		}
		else
		{
			dataPtr = new unsigned char[new_size];
			m_bOwnBuffer = true;
		}

		fileSize = new_size;
	}

public:
	MemStream() : Stream()
	{
		dataPtr = NULL;
		m_bOwnBuffer = true;
		fileSize = 0;
		filePos = 0;
		isReadOnly = false;
		location = "<memory buffer>";
	}

	virtual ~MemStream()
	{
// 		Close();
// 		if (m_bOwnBuffer && m_pData)
// 			delete [] m_pData;
	}

	MemStream(void* data, size_t size) : Stream()
	{
		dataPtr = (unsigned char*)data;
		m_bOwnBuffer = (data == NULL);
		fileSize = m_bOwnBuffer ? 0 : size;
		filePos = 0;
		isOpened = false;
	}

	bool	Open(OpenMode mode)
	{
		if(isOpened) Close();
		isReadOnly = mode == READ;
		isOpened = true;
		filePos = 0;
		return true;
	}

	void	Close()
	{
		if(isOpened == false) return;
		Stream::Close();
		isOpened = false;
	}

	int		ReadBuffer(void *dest, int count)
	{
		ASSERT(isOpened, "File not opened.");

		int _count = count;
		if(filePos + _count > fileSize)
		{
			_count = fileSize - filePos;
		}
		memcpy(dest, dataPtr + filePos, _count);

		filePos += _count;

		return _count;
	}

	int		WriteBuffer(const void *src, int count)
	{
		ASSERT(isOpened, "File not opened.");
		ASSERT(!isReadOnly, "File is readonly.");

		int _count = count;
		if(filePos + _count > fileSize)
		{
			//_count = m_nSize - m_nPos;
			IncreaseSize(filePos + _count - fileSize);
		}

		memcpy(dataPtr + filePos, src, _count);

		filePos += _count;

		return _count;
	}

	bool	Seek(SeekPos pos, int32 count)
	{
		ASSERT(isOpened, "File not opened.");
		ASSERT(count < (int32)fileSize);

		if(pos == SEEK_FROM_CURRENT)
			filePos += count;
		else if(pos == SEEK_FROM_START)
			filePos = count;
		else if(pos == SEEK_FROM_END)
			filePos = fileSize - 1 - count;

		if(filePos >= fileSize) filePos = fileSize - 1;

		return true;
	}

	uint32	Pos()
	{
		ASSERT(isOpened, "File not opened.");
		if(filePos >= fileSize) filePos = fileSize;
		return filePos;
	}

	int32	Size()
	{
		return fileSize;
	}
};

MemoryFileSystem::MemoryFileSystem() : GenericFileSystem()
{

}

MemoryFileSystem::~MemoryFileSystem()
{

}

String MemoryFileSystem::GetDescription()
{
	return "Memory File System";
}

bool MemoryFileSystem::CanHandleProtocol( const char* p )
{
	return strcmp(p, "mem") == 0 || strcmp(p, "memory") == 0;
}

Stream* MemoryFileSystem::GetStream( const char* location )
{
	if(location != NULL)
	{
		if(location[0] == '@') // it's an address
		{
			long addr, size;
			sscanf_s(location, "@%lu,%lu",&addr, &size);

			return new MemStream((void*)addr, size);
		}
	}
	return new MemStream();
}
