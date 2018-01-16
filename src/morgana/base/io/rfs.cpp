#include "rfs.h"
#include "paths.h"
#include "mefiles.h"
#include "debug.h"

using namespace MorganaEngine::Base::IO;

__implement_class(RegistryFileSystem, GenericFileSystem);

RegistryFileSystem::RegistryFileSystem() : GenericFileSystem()
{
	isInited = false;
	isDiskOperation = false;
}

RegistryFileSystem::~RegistryFileSystem()
{
	WriteToDisk();
}

String RegistryFileSystem::GetDescription()
{
	return "Registry File System";
}

bool RegistryFileSystem::CanHandleProtocol( const char* p )
{
	return strcmp(p, "reg") == 0 || strcmp(p, "registry") == 0;
}

Stream* RegistryFileSystem::GetStream( const char* location )
{
	Initialize();
	const unsigned int hash = String::ComputeHashCode(location);
	int found = -1;
	for(int i = 0; i < entries.Length(); i++)
	{
		if(entries[i].name.Equals(location, hash))
		{
			found = i;
			break;
		}
	}

	if(found == -1)
	{
		entries.Realloc(entries.Length() + 1);
		RegistryEntry_t& last = entries.Last();
		last.name = location;
		last.data = singleton<MFiles>()->GetStream("mem://null");
		found = entries.Length() - 1;
	}

	return entries[found].data;
}

void RegistryFileSystem::ReadFromDisk()
{
	entries.SetLength(0);

	Stream* regfile = GetFile();
	if(regfile == NULL) return;
	if(regfile->Open(Stream::READ) == false) return;
	if(regfile->Size() < 4) return;

	isDiskOperation = true;

	regfile->Seek(Stream::SEEK_FROM_START, 0);
	const int count = regfile->ReadUInt32();
	entries.SetLength(count);

	Array<unsigned char> buffer;
	buffer.SetLength(512);

	char name[512];

	for(int i = 0; i < entries.Length(); i++)
	{
		RegistryEntry_t& re = entries[i];
		regfile->ReadString(name);

		re.name = name;
		re.data = singleton<MFiles>()->GetStream("mem://null");
		re.data->Open(Stream::READ_WRITE);

		const unsigned int sz = regfile->ReadUInt32();
		buffer.SetLength(sz);
		regfile->ReadBuffer(buffer.ptr(), sz);
		re.data->WriteBuffer(buffer.ptr(), sz);
		re.data->Seek(Stream::SEEK_FROM_START, 0);
		re.data->Close();
	}

	regfile->Close();

	isDiskOperation = false;
}

void RegistryFileSystem::WriteToDisk()
{
	Stream* regfile = GetFile();
	if(regfile->Open(Stream::CREATE) == false)
	{
		DEBUG_OUT("RegistryFileSystem writing error.");
		return;
	}

	isDiskOperation = true;

	regfile->Seek(Stream::SEEK_FROM_START, 0);
	regfile->WriteUInt32(entries.Length());
	Array<unsigned char> buffer;
	buffer.SetLength(512);
	for(int i = 0; i < entries.Length(); i++)
	{
		RegistryEntry_t& re = entries[i]; 
		
		const unsigned int sz = re.data->Size();
		buffer.SetLength(sz);
		re.data->Open(Stream::READ);
		if(sz > 0)
		{
			re.data->Seek(Stream::SEEK_FROM_CURRENT, 0);
			re.data->ReadBuffer(buffer.ptr(), sz);
		}
		re.data->Close();

		regfile->WriteString(re.name);
		regfile->WriteUInt32(sz);
		if(sz > 0)
			regfile->WriteBuffer(buffer.ptr(), sz);
	}

	regfile->Close();

	isDiskOperation = false;
}

void RegistryFileSystem::Initialize()
{
	if(isInited) return;

	ReadFromDisk();

	isInited = true;
}

Stream* RegistryFileSystem::GetFile()
{
	String path = Paths::ConcatPathWithFilename(Paths::GetSystemFilesPath(), "mr");
	Stream* s = singleton<MFiles>()->GetStream(path);
	return s;
}

void RegistryFileSystem::OnStreamRetrieved( Stream* s )
{

}

void RegistryFileSystem::OnStreamClosed( Stream* s )
{
	if(isDiskOperation == false)
		WriteToDisk();
}
