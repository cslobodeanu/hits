#ifndef __MORGANA_FRAMEWORK_RESOURCES_FONTS_FONT_TRUE_TYPE_H__
#define __MORGANA_FRAMEWORK_RESOURCES_FONTS_FONT_TRUE_TYPE_H__

#include "../resourcefile.h"
#include "devices.h"
#include "binpacker.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Fonts
			{
				class FontTrueType : public ResourceFile
				{
					__declare_class(FontTrueType, ResourceFile);
				public:
					FontTrueType();
					virtual ~FontTrueType();

					const int atlasSize = 1024;

					struct GlyphInfo_t
					{
						Texture*	texture;
						Rect		rect;
						int			size;
						bool		transposed;
						int			offsetLeft, offsetTop;
						int			advanceX;

						uint		glyphIndex;
					};

					enum SpecialSprites
					{
						White,
						Blob,
						SpecialSpritesCount
					};

					GlyphInfo_t*			RequestGlyph(const int character, const int size);
					SpriteInfo_t*			RequestSprite(const SpecialSprites ss, const int size = 16);

					bool					GetGlyphInfo(const int character, const int size, GlyphInfo_t& glyphOut);

					const float				GetKerning(GlyphInfo_t* current, GlyphInfo_t* previous) const;
					const float				GetLineSpacing(const int size);
					const float				GetLineHeight(const int size);

					const void				Append(FontTrueType* other);

					const void				ForceSymbols();

				protected:
					virtual void			ImportFromStreams(const StreamList&  streams, const char* args = NULL);
					virtual const bool		SaveToStream(Stream* s);

					static FT_Library		freeTypeLibrary;
					static bool				freeTypeLibraryTriedToLoad;
					void					InitializeFreeType();

					byte*					fontData;
					FT_Face					face;

					bool					kerning;

					struct GlyphList_t
					{
						Array<unsigned int>	glyphIndices;

						GlyphList_t()
						{
							glyphIndices.SetNullOnAlloc();
						}
					};

					Array<GlyphList_t*>	glyphListsBySize;

					Array<GlyphInfo_t*> allGlyphs;

					typedef BinPacker::GuillotineBinPack Packer;

					struct TexturePack_t
					{
						Texture*	texture;

						Packer		packer;

						TexturePack_t(int size);
						~TexturePack_t()
						{
							SAFE_DEL(texture);
						}
					};
					Array<TexturePack_t*> textures;

					void				CreateGlyphList(const int size);
					void				AllocBlock(const int width, const int height, Texture** outTex, Rect* outRect);

					void				AddNewTexture();

					int					prevRequestedSize;
					void				RequestCharSize(const int size);

					Array<SpriteInfo_t*> specialSprites;

					FontTrueType*		combinedWith;
				};
			}
		}
	}
}

#endif