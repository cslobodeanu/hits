#include "platform.h"

#ifdef OS_WINDOWS_WIN32

#include "meio.h"
#include <sys/stat.h>
#include <Shlobj.h>

using namespace  MorganaEngine::Base::IO;

FileStream::FileStream() : Stream()
{
	file = NULL;
	size = 0;
}

FileStream::~FileStream()
{

}

// open and close
bool FileStream::Open(OpenMode mode)
{
	if(isOpened) Close();
	switch(mode)
	{
	case READ:
		{
			fopen_s(&file, location, "rb");
			break;
		}
	case READ_WRITE:
		{
			fopen_s(&file, location, "r+b");
			break;
		}
	case CREATE:
		{
			fopen_s(&file, location, "wb");		
			break;
		}
	case APPEND:
		{
			fopen_s(&file, location, "a+b");
			break;
		}
	default:
		{
			break;
		}
	}

	if(file == NULL)
		return false;

	//cache the file size
	Size();

	isOpened = true;

	return true;

}

void FileStream::Close()
{
	if (file)
		fclose(file);
	file = NULL;
	Stream::Close();
}

// read from file
int	 FileStream::ReadBuffer(void   *dest, int count)
{
	if(count <= 0 || dest == NULL )
		return 0;

	return fread(dest, 1, count, file);
}

// write to file
int  FileStream::WriteBuffer(const void *src, int count)
{
	if(count <= 0 || src == NULL)
		return 0;

	return fwrite(src, 1, count, file);

}

// parse file	
bool FileStream::Seek(SeekPos pos, int32 count)
{
	uint32 nSeekFrom = 0;
	switch(pos)
	{
	case SEEK_FROM_CURRENT:
		{
			nSeekFrom = SEEK_CUR; 
			break;
		}
	case SEEK_FROM_START:
		{
			nSeekFrom = SEEK_SET; 
			break;
		}
	case SEEK_FROM_END:
		{
			nSeekFrom = SEEK_END; 
			break;
		}
	default:
		{
			// wrong pos
			return false;
		}
	}

	return (!fseek(file, count, nSeekFrom));

}

uint32 FileStream::Pos()
{
	return ftell(file);
}

int32 FileStream::Size()
{
	struct _stat st;

	if (_stat(location, &st) != -1)
	{
		size = st.st_size;
	}
	else
	{
		//path does not exists
		size = Stream::FILE_NOT_FOUND;
	}
	return size;
}

#endif