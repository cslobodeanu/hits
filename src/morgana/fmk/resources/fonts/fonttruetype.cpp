#include "fonttruetype.h"
#include "mememory.h"
#include "../../canvas/canvas.h"
#include "meutils.h"
#include "images/stdimages.h"

#ifdef _DEBUG
#pragma comment(lib, "freetype27d.lib")
#else
#pragma comment(lib, "freetype27.lib")
#endif

using namespace MorganaEngine::Framework::Resources::Fonts;
using namespace MorganaEngine::Framework::Resources::Images;

__implement_class(FontTrueType, ResourceFile);

FT_Library FontTrueType::freeTypeLibrary = NULL;
bool FontTrueType::freeTypeLibraryTriedToLoad = false;

FontTrueType::TexturePack_t::TexturePack_t(int size)
{
	packer.Init(size, size);

	texture = new Texture();
	Texture::CreateParams_t texParams(size, size);
	texParams.format = PixelFormat::FormatGrey;
	texParams.filtering = Filtering::Bilinear;
	implementation<DisplayObjectFactory>()->Create(texture, &texParams);
}


FontTrueType::FontTrueType()
{
	face = NULL;
	fontData = NULL;
	prevRequestedSize = -1;
	glyphListsBySize.SetNullOnAlloc();

	specialSprites.SetNullOnAlloc();
	specialSprites.SetLength(SpecialSpritesCount);

	combinedWith = NULL;
}

FontTrueType::~FontTrueType()
{
	if (face != NULL)
	{
		FT_Done_Face(face);
		SAFE_DEL_ARRAY(fontData);
	}

	for (int i = 0; i < glyphListsBySize.Length(); i++)
	{
		SAFE_DEL(glyphListsBySize[i]);
	}

	for (int i = 0; i < allGlyphs.Length(); i++)
	{
		SAFE_DEL(allGlyphs[i]);
	}

	for (int i = 0; i < textures.Length(); i++)
	{
		SAFE_DEL(textures[i]);
	}

	for (int i = 0; i < specialSprites.Length(); i++)
	{
		SAFE_DEL(specialSprites[i]);
	}
}

void FontTrueType::ImportFromStreams(const StreamList&  streams, const char* args /*= NULL*/)
{
	conversionWasOk = false;

	InitializeFreeType();
	if (freeTypeLibrary == NULL) return;

	Stream* s = streams.First();
	if (s->Open(Stream::READ) == false) return;

	int fsz = s->Size();

	fontData = new byte[fsz];
	s->ReadBuffer(fontData, fsz);
	s->Close();

	FT_Error error = FT_New_Memory_Face(freeTypeLibrary,
										fontData,			/* first byte in memory */
										fsz,				/* size in bytes        */
										0,					/* face_index           */
										&face);


	if (error != FT_Err_Ok)
	{
		DEBUG_OUT("Failed to load font [%s]", streams[0]->location.c_str());
		return;
	}

	if (face->num_charmaps > 0)
		face->charmap = face->charmaps[0];

	name = String::Format("%s %s", face->family_name, face->style_name);

	kerning = FT_HAS_KERNING(face) != 0;

	conversionWasOk = true;
}

const bool FontTrueType::SaveToStream(Stream* s)
{
	return false;
}

void FontTrueType::InitializeFreeType()
{
	if (freeTypeLibraryTriedToLoad) return;

	freeTypeLibraryTriedToLoad = true;

	FT_Error error = FT_Init_FreeType(&freeTypeLibrary);
	if (error != FT_Err_Ok)
	{
		DEBUG_OUT("Failed to initialize FreeType Library");
		return;
	}
}

FontTrueType::GlyphInfo_t* FontTrueType::RequestGlyph(const int character, const int size)
{
	RequestCharSize(size);

	CreateGlyphList(size);
	GlyphList_t* gl = glyphListsBySize[size];

	if (character >= gl->glyphIndices.Length() || gl->glyphIndices[character] == 0)
	{
		FT_UInt  glyph_index = FT_Get_Char_Index(face, character);
		if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) == FT_Err_Ok)
		{
			if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LIGHT) == FT_Err_Ok)
			{
				FT_GlyphSlot  slot = face->glyph;

				//if (slot->bitmap.pitch != 0)
				{
					GlyphInfo_t* gi = new GlyphInfo_t();
					gi->glyphIndex = glyph_index;
					gi->size = size;
					gi->offsetLeft = slot->bitmap_left;
					gi->offsetTop = slot->bitmap_top;
					gi->advanceX = slot->advance.x >> 6;

					if (slot->bitmap.width == 0)
					{
						gi->rect.Set(0, 0, 0, 0);
					}
					else
					{
						const int border = 0.0f;
						const int allocW = slot->bitmap.pitch + 2 * border;
						const int allocH = slot->bitmap.rows + 2 * border;

						AllocBlock(allocW, allocH, &gi->texture, &gi->rect);

						gi->transposed = gi->rect.height != allocH;


						gi->rect.Inflate(-2 * border, -2 * border);

						GrayscaleBuffer b;
						if (gi->transposed)
						{
							b.Alloc(gi->rect.height, gi->rect.width);
							b.BlitChannel(0, slot->bitmap.buffer, 0, 1);
							b.Transpose();
						}
						else
						{
							b.Alloc(gi->rect.width, gi->rect.height);
							b.BlitChannel(0, slot->bitmap.buffer, 0, 1);
						}

						implementation<DisplayObjectFactory>()->UpdateSubresource(gi->texture, gi->rect, 0, (byte*)b);
					}

					allGlyphs.Add(gi);

					if (character >= gl->glyphIndices.Length())
					{
						gl->glyphIndices.SetLength(character + 1);
					}
					gl->glyphIndices[character] = allGlyphs.Length();

					return allGlyphs[gl->glyphIndices[character] - 1];
				}
			}
		}
	}
	else
	{
		return allGlyphs[gl->glyphIndices[character] - 1];
	}

	return NULL;
}

bool FontTrueType::GetGlyphInfo(const int character, const int size, GlyphInfo_t& glyphOut)
{
	RequestCharSize(size);

	FT_UInt  glyph_index = FT_Get_Char_Index(face, character);
	if (glyph_index == 0) return false;

	if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) != FT_Err_Ok) return false;
	if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) != FT_Err_Ok) return false;

	FT_GlyphSlot  slot = face->glyph;

	glyphOut.glyphIndex = glyph_index;
	glyphOut.size = size;
	glyphOut.offsetLeft = slot->bitmap_left;
	glyphOut.offsetTop = slot->bitmap_top;
	glyphOut.advanceX = slot->advance.x >> 6;

	if (slot->bitmap.width == 0)
	{
		glyphOut.rect.Set(0, 0, 0, 0);
	}
	else
	{
		glyphOut.rect.Set(0, 0, slot->bitmap.pitch, slot->bitmap.rows);
	}

	return true;
}

void FontTrueType::CreateGlyphList(const int size)
{
	if (size >= glyphListsBySize.Length())
	{
		glyphListsBySize.SetLength(size + 1);
	}

	GlyphList_t* gl = glyphListsBySize[size];
	if (gl == NULL)
	{
		gl = new GlyphList_t();
		glyphListsBySize[size] = gl;
	}

	return;
}

void FontTrueType::AllocBlock(const int width, const int height, Texture** outTex, Rect* outRect)
{
	Array<TexturePack_t*>& tex = combinedWith ? combinedWith->textures : textures;

	if (tex.Length() == 0)
	{
		tex.Add(new TexturePack_t(atlasSize));
	}

	TexturePack_t* tp = tex.Last();
	*outRect = tp->packer.Insert(width, height, false, Packer::RectBestAreaFit, Packer::SplitShorterLeftoverAxis);
	*outTex = tp->texture;
	if ((*outRect).height == 0)
	{
		tex.Add(new TexturePack_t(atlasSize));
		AllocBlock(width, height, outTex, outRect);
	}
}

const float FontTrueType::GetKerning(GlyphInfo_t* current, GlyphInfo_t* previous) const
{
	if (previous == NULL) return 0;
	if (kerning == false) return 0;

	FT_Vector k;
	FT_Error err = FT_Get_Kerning(face, previous->glyphIndex, current->glyphIndex, FT_KERNING_DEFAULT, &k);

	return (float)(k.x >> 6);
}

const float FontTrueType::GetLineSpacing(const int size)
{
	RequestCharSize(size);
	return (float)(face->size->metrics.height >> 6);
}

const float FontTrueType::GetLineHeight(const int size)
{
	// dumb way to do it
	GlyphInfo_t*gi = RequestGlyph('T', size);
	if (gi == NULL)
		return 0;
	return (float)(gi->transposed ? gi->rect.width : gi->rect.height);
}

void FontTrueType::RequestCharSize(const int size)
{
	if (prevRequestedSize != size)
	{
		FT_Error error = FT_Set_Char_Size(
			face,					/* handle to face object           */
			size * 64,				/* char_width in 1/64th of points  */
			0,						/* char_height in 1/64th of points */
			Screen::dpi,			/* horizontal device resolution    */
			0);						/* vertical device resolution      */

		if (error != FT_Err_Ok)
		{
			DEBUG_OUT("Cannot set char size for font [%s]", name.c_str());
			return;
		}

		prevRequestedSize = size;
	}
}

SpriteInfo_t* FontTrueType::RequestSprite(const SpecialSprites ss, const int size /*= 16*/)
{
	if (specialSprites[ss] != NULL)
		return specialSprites[ss];

	SpriteInfo_t* si = new SpriteInfo_t();

	AllocBlock(size, size, &si->texture, &si->rect);

	RectangleBuffer<1> pb;

	int shrinkCoords = 0;
	
	if (ss == SpecialSprites::White)
	{
		pb = StandardImage::Fill<1>(size, Color255::white);
		shrinkCoords = size / 4;
	}
	else
	if (ss == SpecialSprites::Blob)
	{
		pb = StandardImage::Disc<1>(size, Color255::white, Color255(0, 0, 0, 0), 0.8f);
		si->borderLeft = size / 2;
		si->borderRight = size / 2;
		si->borderTop = size / 2;
		si->borderBottom = size / 2;
	}

	implementation<DisplayObjectFactory>()->UpdateSubresource(si->texture, si->rect, 0, (byte*)pb);

	si->rect.Inflate(-shrinkCoords, -shrinkCoords);

	specialSprites[ss] = si;
	return si;
}

const void FontTrueType::Append(FontTrueType* other)
{
	other->combinedWith = this;
}

const void FontTrueType::ForceSymbols()
{
	if (face->num_charmaps > 0)
		face->charmap = face->charmaps[0];
	return;
}
