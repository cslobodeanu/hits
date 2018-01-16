#ifndef __MORGANA_BASE_DEVICES_DISPLAY_OBJECTS_TEXTURE_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_OBJECTS_TEXTURE_H__

#include "../displayobject.h"
#include "../rendepipelinedefs.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				namespace DisplayObjects
				{
					class Texture : public DisplayObject
					{
						__declare_class(Texture, DisplayObject);
					public:
						Texture();

						struct CreateParams_t
						{
							int		width, height;
							bool	hasMipmaps;

							PixelFormat	format;
							byte		pixelType;
							WrapMode	wrap;
							Filtering	filtering;

							CreateParams_t()
							{
								filtering = Filtering::Bilinear;
								hasMipmaps = false;
								format = PixelFormat::FormatRGBA;
								pixelType = PixelType::UnsignedByte;
								wrap = WrapMode::Repeat;
							}

							CreateParams_t(const int w, const int h, const Filtering filtering = Filtering::Bilinear)
							{
								width = w;
								height = h;
								hasMipmaps = false;
								this->filtering = filtering;
								format = PixelFormat::FormatRGBA;
								pixelType = PixelType::UnsignedByte;
								wrap = WrapMode::Repeat;
							}

							unsigned int GetAllocSize()
							{
								int numBytes = 4;
								if (format == PixelFormat::FormatGrey)
									numBytes = 1;
								if (format == PixelFormat::FormatAlpha)
									numBytes = 1;
								if (format == PixelFormat::FormatGreyAlpha)
									numBytes = 2;
								int sz = width * height * numBytes;
								if(hasMipmaps)
									sz = (int)((float)sz * 1.33f);
								return sz;
							}
						};

						const Rect GetBounds() const;

						const Filtering GetFiltering() const;
						const void		SetFiltering(const Filtering filtering);

						const WrapMode	GetWrapMode() const;
						const void		SetWrapMode(const WrapMode wrapMode);

						const bool HasMipmaps() const;

						static Texture*		DefaultWhite;
						static Texture*		DefaultGray;
						static Texture*		DefaultBlack;
						static Texture*		DefaultRed;
						static Texture*		DefaultGreen;
						static Texture*		DefaultBlue;
						static Texture*		DefaultUp;
						static Texture*		DefaultGradient;
						static Texture*		DefaultBlob;
						static Texture*		DefaultSmallDisc;
						static Texture*		DefaultBigDisc;
						static Texture*		DefaultSmallCircle;
						static Texture*		DefaultCheckered;
						static Texture*		DefaultCheckered16;

						enum
						{
							DEFAULT_TEXTURE_WHITE,
							DEFAULT_TEXTURE_GRAY,
							DEFAULT_TEXTURE_BLACK,
							DEFAULT_TEXTURE_RED,
							DEFAULT_TEXTURE_GREEN,
							DEFAULT_TEXTURE_BLUE,
							DEFAULT_TEXTURE_UNIT_Y,
							DEFAULT_TEXTURE_GRADIENT,
							DEFAULT_TEXTURE_BLOB,
							DEFAULT_TEXTURE_SMALL_DISC,
							DEFAULT_TEXTURE_BIG_DISC,
							DEFAULT_TEXTURE_SMALL_CIRCLE,
							DEFAULT_TEXTURE_CHECKERED2,
							DEFAULT_TEXTURE_CHECKERED16,
							DEFAULT_TEXTURES_COUNT
						};
						static Texture*			CreateBasicTexture(const int which, const char* name, int size = 8);
						static void				InitializeDefaultTextures();
					};
				}
			}
		}
	}
}

#define		SAFE_TEXTURE(t) ( t != NULL ? t : TEXTURE_CHECKERED16 )

#define		TEXTURE_WHITE			(MorganaEngine::Base::Devices::Display::DisplayObjects::Texture::Default(MorganaEngine::Base::Devices::Display::DisplayObjects::Texture::DEFAULT_TEXTURE_WHITE))
#define		TEXTURE_CHECKERED16		(MorganaEngine::Base::Devices::Display::DisplayObjects::Texture::Default(MorganaEngine::Base::Devices::Display::DisplayObjects::Texture::DEFAULT_TEXTURE_CHECKERED16))

#endif