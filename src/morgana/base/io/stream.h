#ifndef __MORGANA_BASE_IO_STREAM_H__
#define __MORGANA_BASE_IO_STREAM_H__

#include <stdio.h>
#include "../compatibility/compatibility.h"
#include "metypes.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class GenericFileSystem;
			typedef Array<byte> RawBuffer;
			class Stream
			{
			protected:
				bool	isOpened;
			public:

				enum OpenMode
				{
					READ,
					READ_WRITE,
					CREATE,
					APPEND
				};

				enum SeekPos
				{
					SEEK_FROM_CURRENT,
					SEEK_FROM_START,
					SEEK_FROM_END
				};

				enum FileSizeType
				{
					STREAM_IN_STANDBY = -100, // necessary for Windows8 - used as default value in constructor for "m_nSize" field
					FILE_NOT_FOUND = -1,
					FOLDER = -2
				};

				Stream();
				virtual ~Stream();

				// open and close
				virtual bool Open(OpenMode mode) = 0;
				virtual void Close();

				String				location;
				GenericFileSystem*	fileSystem;

				// read from file
				virtual int	ReadBuffer(void   *dest, int count) = 0;

				int8	ReadInt8();
				uint8	ReadUInt8();
				int16	ReadInt16();
				uint16	ReadUInt16();
				int32	ReadInt32();
				uint32	ReadUInt32();

				void	ReadInt8(int8& v) { v = ReadInt8(); }
				void	ReadUInt8(uint8& v) { v = ReadUInt8(); }
				void	ReadInt16(int16& v) { v = ReadInt16(); }
				void	ReadUInt16(uint16& v) { v = ReadUInt16(); }
				void	ReadInt32(int32& v) { v = ReadInt32(); }
				void	ReadUInt32(uint32& v) { v = ReadUInt32(); }

				void    ReadString(char*);
				String	ReadString();
				float   ReadFloat();
				void	ReadNullTerminatedString(char*);

				int32 ReadLineCRLF(char*, int32 Size);
				int32 ReadLine(char*, int32 Size);

				//big endian
				int16	ReadInt16BE();
				uint16	ReadUInt16BE();
				int32	ReadInt32BE();
				uint32	ReadUInt32BE();

				// write to file
				virtual int  WriteBuffer(const void *src, int count) = 0;

				bool WriteInt8	(int8   val);
				bool WriteUInt8	(uint8  val);
				bool WriteInt16	(int16  val);
				bool WriteUInt16(uint16 val);
				bool WriteInt32	(int32  val);
				bool WriteUInt32(uint32 val);
				bool WriteString(const char*);
				bool WriteStringBuffer(const char*);
				bool WriteFloat(float val);
				// parse file	
				virtual bool Seek(SeekPos pos, int32 count) = 0;
				virtual uint32 Pos() = 0;
				virtual int32 Size() = 0;

				bool	IsOpened() { return isOpened; }
			};

			typedef Array<Stream*> StreamList;
		}
	}
}
#endif