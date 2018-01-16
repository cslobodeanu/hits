#include "imagefile.h"
#include "imagedecoder.h"
#include "meio.h"
#include "mememory.h"

using namespace MorganaEngine::Framework::Resources::Images;

__implement_class(ImageFile, ResourceFile);

void ImageFile::ImportFromStreams(const StreamList&  streams, const char* args /*= NULL*/)
{
	Stream* s = streams.First();

	width = height = bits = 0;
	conversionWasOk = false;

	Array<CLASSDESC> classes = _Reflection.GetClassesOfType<ImageDecoder>();

	ImageDecoder* dec = NULL;
	for(int i = 0; i < classes.Length(); i++)
	{
		dec = (ImageDecoder*)MEObject::Instantiate(classes[i]);
		if(dec->CanDecode(s))
		{
			ImageDecoder::ImageHeader_t header;
			dec->ReadHeader(s, header);
			width = header.width;
			height = header.height;
			bits = header.bits;
			if(header.width > 0)
			{
				Alloc(header.width, header.height, header.bits, header.mipmaps > 1);
				dec->ReadPixels(s, this);
				conversionWasOk = true;
				SAFE_DEL(dec);
				break;
			}
		}
		SAFE_DEL(dec);
	}
}

const bool ImageFile::SaveToStream(Stream* s)
{
	ImageDecoder* enc = NULL;
	Array<CLASSDESC> classes = _Reflection.GetClassesOfType<ImageDecoder>();
	for(int i = 0; i < classes.Length(); i++)
	{
		enc = (ImageDecoder*)MEObject::Instantiate(classes[i]);
		if(enc->CanEncode(s))
		{
			enc->WritePixels(s, this);
			SAFE_DEL(enc);
			return true;
		}
		SAFE_DEL(enc);
	}
	return false;
}

void ImageFile::Alloc(const int width, const int height, const int bits, const bool mipmaps /*= false*/)
{
	Release();

	this->width = width;
	this->height = height;
	this->bits = bits;

	int w = width;
	int h = height;
	while(w > 0)
	{
		MipMapLevel_t level;
		level.raw = new unsigned char[w * h * bits / 8];
		imageMipmaps.Add(level);

		if(!mipmaps || w != h) break;

		w = w >> 1;
	}
}

void ImageFile::Release()
{
	for(int i = 0; i < imageMipmaps.Length(); i++)
	{
		SAFE_DEL_ARRAY(imageMipmaps[i].raw);
	}

	imageMipmaps.Clear();
}

ImageFile::ImageFile()
{
	width = height = bits = 0;
}

unsigned char* ImageFile::GetPixels(const int level /*= 0*/)
{
	return imageMipmaps[level].raw;
}

unsigned char*& ImageFile::GetPixelsRef(const int level /*= 0*/)
{
	return imageMipmaps[level].raw;
}


void ImageFile::FlipY()
{
	int w = width;
	int h = height;
	LocalMemoryBlock temp(width * 4);
	for(int i = 0; i < imageMipmaps.Length(); i++)
	{
		unsigned char* pixels = (unsigned char*)GetPixels(i);
		const unsigned rowSz = GetRowSize(i);
		for(int y = 0; y < h / 2; y++)
		{
			unsigned char* row1 = pixels + y * rowSz;
			unsigned char* row2 = pixels + (h - 1 - y) * rowSz;
			memcpy(temp, row1, rowSz);
			memcpy(row1, row2, rowSz);
			memcpy(row2, temp, rowSz);
		}

		w = w >> 1;
		h = h >> 1;
	}
}

const unsigned int ImageFile::GetRowSize(const int level /*= 0*/) const
{
	const int w = width >> level;
	return w * bits / 8;
}
