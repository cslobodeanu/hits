#include "textdata.h"
#include "base.h"

using namespace MorganaEngine::Framework::Resources;

__implement_class(TextData, ResourceFile);

TextData::TextData()
{
}

void TextData::ImportFromStreams(const StreamList&  streams, const char* args /*= NULL*/)
{
	text = "";

	conversionWasOk = false;

	SetNameFromPath(streams);

	if (streams.Length() == 0)
	{
		return;
	}

	if (streams.Length() == 0) return;

	Stream* s = streams[0];
	s->Open(Stream::READ);

	LocalMemoryBlock lmb(s->Size() + 1);
	lmb[s->Size()] = 0;
	s->ReadBuffer((char*)lmb, s->Size());

	text = lmb.c_str();

	s->Close();

	conversionWasOk = true;
}

const bool TextData::SaveToStream(Stream* s)
{
	if (s->Open(Stream::CREATE) == false) return false;

	s->WriteBuffer(text.c_str(), text.Length());
	s->Close();
	return true;
}