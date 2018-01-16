#ifndef __MORGANA_FMK_RESOURCES_IMAGES_IMAGE_DECODER_H__
#define __MORGANA_FMK_RESOURCES_IMAGES_IMAGE_DECODER_H__

#include "reflection.h"
#include "meio.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Images
			{
				class ImageFile;
				class ImageDecoder : public MEObject
				{
					__declare_class_abstract(ImageDecoder, MEObject);
				public:
					ImageDecoder() {}
					virtual ~ImageDecoder() {}

					struct ImageHeader_t
					{
						unsigned int		width, height, bits, mipmaps;
					};

					virtual bool	CanDecode(Stream* s) = 0;
					virtual bool	CanEncode(Stream* s) { return false; }

					virtual void	ReadHeader(Stream* s, ImageHeader_t& header) = 0;
					virtual void	ReadPixels(Stream* s, ImageFile* img) = 0;

					virtual void	WritePixels(Stream* s, ImageFile* img) {};
				};
			}
		}
	}
}

#endif