#ifndef __MORGANA_FMK_RESOURCES_IMAGES_DECODERS_PNG_DECODER_H__
#define __MORGANA_FMK_RESOURCES_IMAGES_DECODERS_PNG_DECODER_H__

#include "../imagedecoder.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Images
			{
				namespace Decoders
				{
					class PNGDecoderEncoder : public ImageDecoder
					{
						__declare_class(PNGDecoderEncoder, ImageDecoder);
					public:

						virtual bool	CanDecode(Stream* s);
						virtual bool	CanEncode(Stream* s);

						virtual void	ReadHeader(Stream* s, ImageHeader_t& header);
						virtual void	ReadPixels(Stream* s, ImageFile* img);

						virtual void	WritePixels(Stream* s, ImageFile* img);
					};
				}
			}
		}
	}
}

#endif