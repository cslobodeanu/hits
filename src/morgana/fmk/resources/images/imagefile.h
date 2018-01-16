#ifndef __MORGANA_FMK_RESOURCES_IMAGES_IMAGEFILE_H__
#define __MORGANA_FMK_RESOURCES_IMAGES_IMAGEFILE_H__

#include "../resourcefile.h"


namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Images
			{
				class ImageFile: public ResourceFile
				{
					__declare_class(ImageFile, ResourceFile);
				public:

					ImageFile();

					void			Alloc(const int width, const int height, const int bits, const bool mipmaps = false);
					virtual void	Release();

					void			FlipY();

					int				width, height, bits;

					unsigned char*			GetPixels(const int level = 0);
					unsigned char*&			GetPixelsRef(const int level = 0);
					const unsigned int		GetRowSize(const int level = 0) const;

				protected:
					virtual void		ImportFromStreams(const StreamList&  streams, const char* args = NULL);
					virtual const bool	SaveToStream(Stream* s);

					struct MipMapLevel_t
					{
						unsigned char*	raw;
					};

					Array<MipMapLevel_t>	imageMipmaps;

				};
			}
		}
	}
}

#endif