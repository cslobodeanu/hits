#include "rawdata.h"

using namespace MorganaEngine::Framework::Resources;

__implement_class(RawData, ResourceFile);

RawData::RawData()
{
}

void RawData::ImportFromStreams(const StreamList&  streams, const char* args /*= NULL*/)
{
	bytes.Clear();
	conversionWasOk = false;

	SetNameFromPath(streams);

	if (streams.Length() == 0)
	{
		return;
	}

	Stream* s = streams[0];
	s->Open(Stream::READ);
	bytes.SetLength(s->Size());
	s->ReadBuffer(bytes.ptr(), bytes.Length());
	s->Close();

	conversionWasOk = true;
}

const bool RawData::SaveToStream(Stream* s)
{
	if (s->Open(Stream::CREATE) == false) return false;

	s->WriteBuffer(bytes.ptr(), bytes.Length());
	s->Close();
	return true;
}