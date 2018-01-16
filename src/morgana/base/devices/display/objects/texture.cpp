#include "texture.h"
#include "../../../memory/localmemoryblock.h"
#include "../displayobjectfactory.h"
#include "../displaydevice.h"

using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Devices::Display::DisplayObjects;
using namespace MorganaEngine::Base::Memory;

__implement_class(Texture, DisplayObject);

Texture*		Texture::DefaultWhite = NULL;
Texture*		Texture::DefaultGray = NULL;
Texture*		Texture::DefaultBlack = NULL;
Texture*		Texture::DefaultRed = NULL;
Texture*		Texture::DefaultGreen = NULL;
Texture*		Texture::DefaultBlue = NULL;
Texture*		Texture::DefaultUp = NULL;
Texture*		Texture::DefaultGradient = NULL;
Texture*		Texture::DefaultBlob = NULL;
Texture*		Texture::DefaultSmallDisc = NULL;
Texture*		Texture::DefaultBigDisc = NULL;
Texture*		Texture::DefaultSmallCircle = NULL;
Texture*		Texture::DefaultCheckered = NULL;
Texture*		Texture::DefaultCheckered16 = NULL;

Texture::Texture() : DisplayObject(DISPLAY_OBJECT_TEXTURE, BINDING_TEXTURE_2D)
{
}

const Rect Texture::GetBounds() const
{
	return Rect(0, 0, __CreateParams->width, __CreateParams->height);
}

const Filtering Texture::GetFiltering() const
{
	return __CreateParams->filtering;
}

const WrapMode Texture::GetWrapMode() const
{
	return __CreateParams->wrap;
}

const bool Texture::HasMipmaps() const
{
	return __CreateParams->hasMipmaps;
}

Texture* Texture::CreateBasicTexture(const int which, const char* name, int size/* = 8*/)
{
	Texture* tex = new Texture();
	tex->SetName(name);
	Rect texRect(0, 0, size, size);

	LocalMemoryBlock buf(size * size * 4);

	if (which <= DEFAULT_TEXTURE_UNIT_Y)
	{
		const Color255 colors[] = { Color255::white, Color255::gray, Color255::black, Color255::red, Color255::green, Color255::blue, Color255::normal_up };

		buf.Fill(&colors[which].r, 4);
	}
	else
		if (which == DEFAULT_TEXTURE_GRADIENT)
		{
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				const float f = (float)y / (size - 1);
				Color255 c = Color255::Lerp(Color255::black, Color255::white, f);
				const int index = (x + y * size) * 4;
				memcpy(buf + index, &c.r, 4);
			}
		}
		}
		else
			if (which == DEFAULT_TEXTURE_BLOB)
			{
		const float half = (float)size * 0.5f;
		const Vector2 center(half, half);

		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				const Vector2 p = Vector2((float)x, (float)y);
				const float d = Math::Mathf::Clamp(Vector2::Distance(&p, &center), 0.0f, half) / half;
				const float f = Math::Mathf::Pow(1.0f - d, 2.0f);
				memset(buf + (x + y * size) * 4 + 3, (byte)(f * 255.0f), 1);
			}
		}
			}
			else
				if (which == DEFAULT_TEXTURE_SMALL_DISC || which == DEFAULT_TEXTURE_BIG_DISC)
				{
		const float half = (float)size * 0.5f;
		const Vector2 center(half, half);

		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				const Vector2 p = Vector2((float)x, (float)y);
				float d = Math::Mathf::Clamp(Vector2::Distance(&p, &center), 0.0f, half) / half;
				const float threshold = which == DEFAULT_TEXTURE_BIG_DISC ? 0.05f : 0.2f;
				d = Mathf::Clamp01((Mathf::Clamp(d, 1.0f - threshold, 1.0f) - (1.0f - threshold)) / threshold);
				const float f = Math::Mathf::Pow(1.0f - d, 2.0f);
				memset(buf + (x + y * size) * 4 + 3, (byte)(f * 255.0f), 1);
			}
		}
				}
				else
					if (which == DEFAULT_TEXTURE_SMALL_CIRCLE)
					{
		const float half = (float)size * 0.5f;
		const Vector2 center(half, half);

		const float borderSize = 4.0f;

		const float realRadius = half - borderSize / 2.0f;

		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				const Vector2 p = Vector2((float)x, (float)y);
				float d = Math::Mathf::Clamp(Math::Mathf::Abs(Vector2::Distance(&p, &center) - realRadius), 0.0f, borderSize * 0.5f) / (borderSize * 0.5f);
				const float f = Math::Mathf::Pow(1.0f - d, 2.0f);
				memset(buf + (x + y * size) * 4 + 3, (byte)(f * 255.0f), 1);
			}
		}
					}
					else
						if (which == DEFAULT_TEXTURE_CHECKERED2 || which == DEFAULT_TEXTURE_CHECKERED16)
						{
		const int sqsz = which == DEFAULT_TEXTURE_CHECKERED2 ? size / 2 : size / 16;
		bool greysq = true;
		for (int y = 0; y < size; y++)
		{
			const int index_y = y / sqsz;
			for (int x = 0; x < size; x++)
			{
				const int index_x = x / sqsz;

				const Color255 c = ((index_x % 2 == 0 && index_y % 2 == 0) || (index_x % 2 == 1 && index_y % 2 == 1)) ? Color255::gray : Color255::white;
				memcpy(buf + 4 * (x + size * y), &c.r, 4);
			}
		}
						}
						else
						{
							NOT_IMPLEMENTED;
						}

	implementation<DisplayObjectFactory>()->Create(tex, &CreateParams_t(size, size));
	implementation<DisplayObjectFactory>()->UpdateSubresource(tex, texRect, 0, buf);

	return tex;
}

#define CREATE_DEF_TEX(which, size)		CreateBasicTexture(which, #which, size)

void Texture::InitializeDefaultTextures()
{
	DefaultWhite = CREATE_DEF_TEX(DEFAULT_TEXTURE_WHITE, 4);
	DefaultGray = CREATE_DEF_TEX(DEFAULT_TEXTURE_GRAY, 4);
	DefaultBlack = CREATE_DEF_TEX(DEFAULT_TEXTURE_BLACK, 4);
	DefaultRed = CREATE_DEF_TEX(DEFAULT_TEXTURE_RED, 4);
	DefaultGreen = CREATE_DEF_TEX(DEFAULT_TEXTURE_GREEN, 4);
	DefaultBlue = CREATE_DEF_TEX(DEFAULT_TEXTURE_BLUE, 4);
	DefaultUp = CREATE_DEF_TEX(DEFAULT_TEXTURE_UNIT_Y, 4);
	DefaultGradient = CREATE_DEF_TEX(DEFAULT_TEXTURE_GRADIENT,32);
	DefaultBlob = CREATE_DEF_TEX(DEFAULT_TEXTURE_BLOB, 32);
	DefaultSmallDisc = CREATE_DEF_TEX(DEFAULT_TEXTURE_SMALL_DISC, 16);
	DefaultBigDisc = CREATE_DEF_TEX(DEFAULT_TEXTURE_BIG_DISC, 64);
	DefaultSmallCircle = CREATE_DEF_TEX(DEFAULT_TEXTURE_SMALL_CIRCLE, 16);
	DefaultCheckered = CREATE_DEF_TEX(DEFAULT_TEXTURE_CHECKERED2, 4);
	DefaultCheckered16 = CREATE_DEF_TEX(DEFAULT_TEXTURE_CHECKERED16, 64);
	DisplayDevice::main->SetTextureFiltering(Filtering::Point);
}

const void Texture::SetFiltering(const Filtering filtering)
{
	__CreateParams->filtering = filtering;
}

const void Texture::SetWrapMode(const WrapMode wrapMode)
{
	__CreateParams->wrap = wrapMode;
}