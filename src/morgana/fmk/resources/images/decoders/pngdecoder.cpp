#include "pngdecoder.h"
#include "meio.h"
#include "mememory.h"
#include "lodepng/lodepng.h"
#include "../imagefile.h"

using namespace MorganaEngine::Framework::Resources::Images::Decoders;
using namespace MorganaEngine::Framework::Resources::Images;
using namespace MorganaEngine::Base::IO;

__implement_class(PNGDecoderEncoder, ImageDecoder);

bool PNGDecoderEncoder::CanDecode(Stream* s)
{
	String ext = Paths::GetExtension(s->location);
	return ext.EqualsNC("png");
}

bool PNGDecoderEncoder::CanEncode(Stream* s)
{
	String ext = Paths::GetExtension(s->location);
	return ext.EqualsNC("png");
}

void PNGDecoderEncoder::ReadHeader(Stream* s, ImageHeader_t& header)
{
	if(s->Open(Stream::READ) == false)
	{
		header.width = 0;
		header.height = 0;
		header.mipmaps = false;
		header.bits = 0;
		return;
	}

	const int fsz = s->Size();

	LocalMemoryBlock buf(fsz);
	s->ReadBuffer(buf, fsz);
	s->Close();
	
	LodePNGState state; memset(&state, 0, sizeof(LodePNGState));
	if(lodepng_inspect(&header.width, &header.height, &state, buf.ptr(), buf.size) == 0)
	{
		header.mipmaps = 1;
		if(state.info_png.color.colortype == LCT_RGBA)
			header.bits = 32;
		if(state.info_png.color.colortype == LCT_RGB)
			header.bits = 24;
		if(state.info_png.color.colortype == LCT_GREY)
			header.bits = 8;
		if(state.info_png.color.colortype == LCT_GREY_ALPHA)
			header.bits = 16;
	}
}

void PNGDecoderEncoder::ReadPixels(Stream* s, ImageFile* img)
{
	if(s->Open(Stream::READ) == false)
	{
		return;
	}

	const int fsz = s->Size();

	LocalMemoryBlock buf(fsz);
	s->ReadBuffer(buf, fsz);
	s->Close();

	unsigned int w, h;
	unsigned char*& ptr = img->GetPixelsRef();
	lodepng_decode_memory(&ptr, &w, &h, buf.ptr(), buf.size, LCT_RGBA, 8);
}

void PNGDecoderEncoder::WritePixels(Stream* s, ImageFile* img)
{
	if(s->Open(Stream::CREATE) == false)
	{
		return;
	}

	unsigned char* buf = NULL;
	unsigned int outsz;

	LodePNGColorType ct = LCT_RGBA;
	if(img->bits == 24) ct = LCT_RGB;
	lodepng_encode_memory(&buf, &outsz, img->GetPixels(0), img->width, img->height, ct, 8);

	s->WriteBuffer(buf, outsz);

	s->Close();

	free(buf);
}
