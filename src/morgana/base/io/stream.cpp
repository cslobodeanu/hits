#include <stdio.h>
#include "stream.h"
#include <string.h>
#include "debug.h"
#include "mefiles.h"
#include "mememory.h"

using namespace MorganaEngine::Base::IO;

Stream::Stream()
{
	fileSystem = NULL;
	isOpened = false;
}

Stream::~Stream()
{
	Close();

	singleton<MFiles>()->OnStreamDeleted(this);
}

void Stream::Close()
{
	if(isOpened)
		singleton<MFiles>()->OnStreamClosed(this);
	isOpened = false;
}

int8 Stream::ReadInt8()
{	
	int8 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(int8));
	ASSERT(nRetValue == sizeof(int8), "Stream::ReadInt8 error");

	return dest;
}

uint8 Stream::ReadUInt8()
{
	uint8 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(uint8));
	ASSERT(nRetValue == sizeof(uint8), "Stream::ReadUInt8 error");

	return dest;
}

int16 Stream::ReadInt16()
{
	int16 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(int16));
	ASSERT(nRetValue == sizeof(int16), "Stream::ReadInt16 error");

	return dest;

}

uint16 Stream::ReadUInt16()
{
	uint16 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(uint16));
	ASSERT(nRetValue == sizeof(uint16), "Stream::ReadUInt16 error");

	return dest;

}

int32 Stream::ReadInt32()
{
	int32 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(int32));
	ASSERT(nRetValue == sizeof(int32), "Stream::ReadInt32 error");

	return dest;
}

uint32 Stream::ReadUInt32()
{
	uint32 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(uint32));
	ASSERT(nRetValue == sizeof(uint32), "Stream::ReadUInt32 error");

	return dest;

}

int16 Stream::ReadInt16BE()
{
	int16 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(int16));
	ASSERT(nRetValue == sizeof(int16), "Stream::ReadInt16 error");

	uint8 *buf = (uint8*)&dest;uint8 tmp = buf[0]; buf[0] = buf[1]; buf[1] = tmp;
	return dest;

}

uint16 Stream::ReadUInt16BE()
{
	uint16 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(uint16));
	ASSERT(nRetValue == sizeof(uint16), "Stream::ReadUInt16 error");

	uint8 *buf = (uint8*)&dest;uint8 tmp = buf[0]; buf[0] = buf[1]; buf[1] = tmp;
	return dest;

}

int32 Stream::ReadInt32BE()
{
	int32 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(int32));
	ASSERT(nRetValue == sizeof(int32), "Stream::ReadInt32 error");

	uint8 *buf = (uint8*)&dest;uint8 tmp = buf[0]; buf[0] = buf[3]; buf[3] = tmp; tmp = buf[1]; buf[1] = buf[2]; buf[2] = tmp;
	return dest;
}

uint32 Stream::ReadUInt32BE()
{
	uint32 dest;

	int nRetValue = ReadBuffer(&dest, sizeof(uint32));
	ASSERT(nRetValue == sizeof(uint32), "Stream::ReadUInt32 error");

	uint8 *buf = (uint8*)&dest;uint8 tmp = buf[0]; buf[0] = buf[3]; buf[3] = tmp; tmp = buf[1]; buf[1] = buf[2]; buf[2] = tmp;
	return dest;

}

void Stream::ReadString(char* buf)
{
	const int len = ReadUInt8();
	if(len > 0) ReadBuffer(buf, len);
	buf[len] = 0;
}

String Stream::ReadString()
{
	const int len = ReadUInt8();
	LocalMemoryBlock buf(len + 1);
	if(len > 0) ReadBuffer(buf, len);
	buf[len] = 0;
	return String(buf);
}

void Stream::ReadNullTerminatedString(char* buf)
{
	int index = 0;
	unsigned char c = ReadUInt8();
	while(c != 0)
	{
		buf[index++] = c;
		c = ReadUInt8();
	}

	buf[index] = 0;
}

int32 Stream::ReadLineCRLF(char* buf, int32 size)
{
	int index = 0;
	size --;
	int32 maxPos = Size() - 1;
	unsigned char c = 0;
	while( (int32)Pos() < maxPos && (index < size))
	{
		c = ReadUInt8();
		if (c != '\r' && c != '\n')
		{
			buf[index++] = c;
		}
		else
		{
			break;
		}
	}

	buf[index] = 0;

	if ((int32)Pos() < maxPos && c == '\r')
	{
		ReadUInt8();
	}

	return index;
}

int32 Stream::ReadLine(char* buf, int32 size)
{
	int index = 0;

	size --;

	unsigned char c = ReadUInt8();
	while( (c != 10) && (c != 13) && ((int32)Pos() < Size()) && (index < size - 1 ) )
	{
		buf[index++] = c;
		c = ReadUInt8();
	}

	buf[index++] = c;

	if (c == 13 && (int32)Pos() < Size())
		ReadUInt8();
	buf[index] = 0;

	return index;
}

bool Stream::WriteInt8(int8   val)
{
	return (WriteBuffer(&val, sizeof(int8)) == sizeof(int8));
}

bool Stream::WriteUInt8(uint8  val)
{
	return (WriteBuffer(&val, sizeof(uint8)) == sizeof(uint8));
}

bool Stream::WriteInt16(int16  val)
{
	return (WriteBuffer(&val, sizeof(int16)) == sizeof(int16));
}

bool Stream::WriteUInt16(uint16 val)
{
	return (WriteBuffer(&val, sizeof(uint16)) == sizeof(uint16));

}

bool Stream::WriteInt32(int32  val)
{
	return (WriteBuffer(&val, sizeof(int32)) == sizeof(int32));
}

bool Stream::WriteUInt32(uint32 val)
{
	return (WriteBuffer(&val, sizeof(uint32)) == sizeof(uint32));
}
bool Stream::WriteString(const char* str)
{
	if(str != NULL)
	{
		const int len = strlen(str);
		if(WriteUInt8(len) == false) return false;
		if(WriteBuffer(str, len) == false) return false;
	}
	else
		WriteUInt8(0);

	return true;
}

float Stream::ReadFloat()
{
	float dest;
	int nRetValue = ReadBuffer((unsigned char*)&dest, sizeof(float));
	return dest;
}

bool Stream::WriteFloat(float val)
{
	return (WriteBuffer(&val, sizeof(float)) == sizeof(float));
}

bool Stream::WriteStringBuffer(const char* str)
{
	if(str != NULL && strlen(str) > 0)
	{
		const int len = strlen(str);
		if(WriteBuffer(str, len) == false) return false;
	}

	return true;
}

void Stream::ToByteBuffer(LocalMemoryBlock& lmb)
{
	const int count = Size();
	if (count == 0) return;
	if (Open(OpenMode::READ) == false) return;

	ReadBuffer(lmb.Realloc(count), count);

	Close();
}

void Stream::ToNullTerminatedString(LocalMemoryBlock& lmb)
{
	const int count = Size();
	if (count == 0) return;
	if (Open(OpenMode::READ) == false) return;

	ReadBuffer(lmb.Realloc(count + 1), count);

	lmb[count] = 0;

	Close();
}

