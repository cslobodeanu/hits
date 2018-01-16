#include "mefiles.h"
#include "stream.h"
#include "paths.h"
#include "metypes.h"
#include "reflection.h"
#include "fsa.h"
#include "mememory.h"
#include "debug.h"

using namespace MorganaEngine::Base::IO;

MFiles::MFiles()
{
}

MFiles::~MFiles()
{
	for(int i = 0; i < fileSystems.Length(); i++)
	{
		GenericFileSystem* fsa = fileSystems[i];
		SAFE_DEL(fsa);
	}
}

Stream* MFiles::GetStream( const char* location )
{
	GenericFileSystem* fsa = GetProperFileSystemForLocation(location);
	if(!fsa) return NULL;
	String loc = Paths::RemoveProtocolPrefix(location);
	Stream* s = fsa->GetStream(loc);
	if(s)
	{
		s->location = loc;
		s->fileSystem = fsa;
		fsa->OnStreamRetrieved(s);
		streams.PushUniqueNoNULL(s);
	}
	return s;
}

GenericFileSystem* MFiles::GetProperFileSystemForLocation( const char* location )
{
	String p = Paths::GetProtocol(location);

	if(fileSystems.Length() == 0)
	{
		Array<CLASSDESC> list = _Reflection.GetClassesOfType<GenericFileSystem>();

		for(int i = 0; i < list.Length(); i++)
		{
			GenericFileSystem* fsa = (GenericFileSystem*)MEObject::Instantiate(list[i]);
			fileSystems.Add(fsa);
		}

		DEBUG_OUT("File systems inited (%d)", list.Length());
		for(int i = 0; i < fileSystems.Length(); i++)
		{
			DEBUG_OUT(">> %s", fileSystems[i]->GetDescription().c_str());
		}

	}

	for(int i = 0; i < fileSystems.Length(); i++)
	{
		if(fileSystems[i]->CanHandleProtocol(p))
		{
			return fileSystems[i];
		}
	}

	return NULL;
}

void MFiles::CopyStream( const char* src, const char* dst )
{
	Stream* s1 = GetStream(src);
	Stream* s2 = GetStream(dst);
	if(s1 == NULL || s2 == NULL) return;

	s1->Open(Stream::CREATE);
	s2->Open(Stream::READ);

	const int sz = s2->Size();
	LocalMemoryBlock buffer(s2->Size());
	s2->ReadBuffer(buffer, sz);
	s1->WriteBuffer(buffer, sz);

	SAFE_DEL(s1);
	SAFE_DEL(s2);
}

void MFiles::Cleanup()
{
	while(streams.Length())
	{
		int l = streams.Length();
		Stream* s = streams.Last();
		SAFE_DEL(s);
		ASSERT( l == streams.Length() + 1 );
	}

	streams.Alloc(0);
}

void MFiles::OnStreamClosed( Stream* s )
{
	if(s->fileSystem)
		s->fileSystem->OnStreamClosed(s);
}

void MFiles::OnStreamDeleted( Stream* s )
{
	if(s->fileSystem)
		s->fileSystem->OnStreamDeleted(s);
	streams.Remove(s);
}

DateTime_t MFiles::GetFileTimeStamp(const char* location)
{
	DateTime_t ts;
	memset(&ts, 0, sizeof(DateTime_t));

#ifdef OS_WINDOWS
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(location, &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		SYSTEMTIME st;
		FileTimeToSystemTime(&ffd.ftLastWriteTime, &st);

		ts.day = (char)st.wDay;
		ts.month = (char)st.wMonth;
		ts.year = st.wYear;
		ts.hour = (char)st.wHour;
		ts.minute = (char)st.wMinute;
		ts.sec = (char)st.wSecond;

		FindClose(hFind);
	}
#endif
	return ts;

}
