#include "texttruetype.h"

using namespace MorganaEngine::Framework::Canvas;

__implement_component(TextTrueType, RendersToCanvas);


TextTrueType::TextTrueType()
{
	text = "Brown fox jumps over lazy dog";
	fontSize = 12;
	charSpacing = 1.0f;
	lineSpacing = 1.0f;
	overflow = false;
	alignment = TextHorizontalAlignment::AlignLeft;
	verticalAlignment = TextVerticalAlignment::AlignTop;
	fontItalic = false;
	font = NULL;

	fontGlow = false;
	fontGlowColor = Color::black;
	fontGlowOffset = Vector2::Zero;

	lines.SetNullOnAlloc();

	mustUpdateQuads = true;

	DoAddProperties();
}

TextTrueType::~TextTrueType()
{
	for (int i = 0; i < lines.GetAllocatedLength(); i++)
	{
		SAFE_DEL(lines[i]);
	}
}

void TextTrueType::OnPreRenderObject()
{
	if (mustUpdateQuads)
	{
		ComputeLines();
		mustUpdateQuads = false;
	}
}

void TextTrueType::Start()
{
	if (material == NULL)
		material = Material::DefaultFontCanvas;
}

void TextTrueType::OnRenderObject()
{
	//__DrawDebugBackground();

	if (font == NULL) return;

	if (lines.Length() > 0)
	{
		FontTrueType::GlyphInfo_t* gi = font->RequestGlyph(text.ToString()[0], fontSize);

		safeMaterial()->SetTexture(gi->texture);

		Vector2 origin = Vector2::Zero;

		if (verticalAlignment == TextVerticalAlignment::AlignCenter)
		{
			origin.y = (rectTransform->rect->height - preferredSize.y) * 0.5f;
		}
		else
		if (verticalAlignment == TextVerticalAlignment::AlignBottom)
		{
			origin.y = rectTransform->rect->height - preferredSize.y;
		}

		if (isBigText)
		{
			BeginBigDataChunk();
		}

		for (int i = 0; i < lines.Length(); i++)
		{
			const float boundsTop = lines[i]->boundsTopLeft.y + origin.y;
			const float boundsBottom = lines[i]->boundsBottomRight.y + origin.y;
			if (boundsTop > rectTransform->rect->height || boundsBottom < 0)
				continue;

			if (lines[i]->quads.Length() == 0) continue;

			Vector2 ofs = Vector2::Zero;
			if (alignment == TextHorizontalAlignment::AlignMiddle)
			{
				ofs.x = (rectTransform->rect->width - (lines[i]->boundsBottomRight.x - lines[i]->boundsTopLeft.x)) * 0.5f;
			}
			else
			if (alignment == TextHorizontalAlignment::AlignRight)
			{
				ofs.x = rectTransform->rect->width - (lines[i]->boundsBottomRight.x - lines[i]->boundsTopLeft.x);
			}

			if (isBigText)
			{
				if (lines[i]->cachedQuadVertices.Length() == 0)
				{
					Array<CanvasSurface::VertexFormat>& cache = lines[i]->cachedQuadVertices;
					cache.SetLength(4 * lines[i]->quads.Length());

					CanvasSurface::VertexFormat* buf = cache.ptr();

					PushQuads(lines[i]->quads.ptr(), lines[i]->quads.Length(), origin + ofs, (void**)&buf);
				}

				PushCachedQuads(lines[i]->cachedQuadVertices.ptr(), lines[i]->quads.Length());
			}
			else
			{
				PushQuads(lines[i]->quads.ptr(), lines[i]->quads.Length(), origin + ofs);
			}
		}

		if (isBigText)
		{
			EndBigDataChunk();
		}
	}
}

void TextTrueType::OnPostRenderObject()
{

}

void TextTrueType::OnPropertySet(_propertyDef* atr)
{
	super::OnPropertySet(atr);

	if (atr == &text)
		isBigText = text->Length() > 64;
	mustUpdateQuads = true;
}

void TextTrueType::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(text);
	__add_property1(fontSize);
	__add_property1(charSpacing);
	__add_property1(lineSpacing);
	__add_property1(overflow);
	__add_property1(alignment);
	__add_property1(verticalAlignment);

	__add_property1(font);
	__add_property1(fontGlow);
	__add_property1(fontGlowColor);
	__add_property1(fontGlowOffset);
}

void TextTrueType::Update()
{

}

const void TextTrueType::NewLine(Vector2& pen)
{
	pen.x = 0.0f;
	pen.y += font->GetLineSpacing(fontSize) * lineSpacing;
}

const Vector2 TextTrueType::GetPreferredSize() const
{
	return preferredSize;
}

void TextTrueType::OnResizeRect()
{
	if (overflow)
		mustUpdateQuads = true;
}

void TextTrueType::ComputeLines()
{
	lines.Clear();

	if (font == NULL) return;
	if (text->Length() == 0) return;

	const float stdLineHeight = font->GetLineHeight(fontSize);

	Vector2 pen = Vector2::Zero;
	pen.y = stdLineHeight;

	FontTrueType::GlyphInfo_t* gi = NULL;
	FontTrueType::GlyphInfo_t* prev = NULL;

	int lastSpaceIndex = -1;
	int lastSpaceQuadIndex = -1;
	Vector2 lastSpaceMaxSize;

	Vector2 boundsMin = Vector2::One * Mathf::MaxFloat;
	Vector2 boundsMax = -boundsMin;

	NewLineInfo();

	FontTrueType::GlyphInfo_t spaceGlyph;
	font->GetGlyphInfo(' ', fontSize, spaceGlyph);

	String txt = text;

	for (int i = 0; i < txt.Length(); i++)
	{
		LineInfo_t* currentLine = lines.Last();
		currentLine->chars += txt[i];

		if (txt[i] == ' ')
		{
			lastSpaceIndex = i;
			lastSpaceQuadIndex = currentLine->quads.Length();
			lastSpaceMaxSize = currentLine->boundsBottomRight;
		}
		else
			if (txt[i] == '\n')
			{
				NewLine(pen);
				NewLineInfo();
				
				lastSpaceIndex = -1;
				lastSpaceQuadIndex = -1;

				continue;
			}
			else
				if (txt[i] == '\r')
				{
					continue;
				}
				else
					if (txt[i] == '\t')
					{
						pen.x += spaceGlyph.advanceX * charSpacing * 5.0f;
						continue;
					}


		gi = font->RequestGlyph(txt[i], fontSize);
		if (gi == NULL) continue;

		if (!overflow)
		{
			const float newAdv = pen.x + (float)gi->advanceX * charSpacing;
			if (newAdv > rectTransform->rect->width)
			{
				NewLine(pen);
				NewLineInfo();

				if (lastSpaceIndex > 0)
				{
					currentLine->quads.SetLength(lastSpaceQuadIndex);
					i = lastSpaceIndex;
					lastSpaceIndex = -1;
					lastSpaceQuadIndex = -1;
					currentLine->boundsBottomRight = lastSpaceMaxSize;
					continue;
				}
			}
		}

		if (gi->rect.width > 0)
		{
			Rectf texR((float)gi->rect.x, (float)gi->rect.y, (float)gi->rect.width, (float)gi->rect.height);
			texR.Normalize((float)gi->texture->GetBounds().width);

			const float kerning = font->GetKerning(gi, prev);

			if (fontGlow)
			{
				currentLine->quads.IncreaseSize(1);

				const float scaleQuad = 2.3f;

				QuadInfo_t& q = currentLine->quads.Last();
				q.origin = pen + Vector2((float)gi->offsetLeft + kerning, -(float)gi->offsetTop);
				q.width = gi->rect.width * scaleQuad;
				q.height = gi->rect.height * scaleQuad;

				Vector2 extraOfs = (Vector2((float)q.width, (float)q.height) - Vector2((float)gi->rect.width, (float)gi->rect.height)) * 0.5f;
				if (gi->transposed)
				{
					const float tmp = extraOfs.x;
					extraOfs.x = extraOfs.y;
					extraOfs.y = tmp;
				}

				q.origin -= extraOfs;
				q.origin += Vector2::Up * (float)fontSize * 0.1f + fontGlowOffset;

				q.texCoord = texR;
				q.transposed = gi->transposed;
				q.color = fontGlowColor->ToColor255();
				q.color2 = Color255(255, 0, 0, 0);
			}

			currentLine->quads.IncreaseSize(1);

			QuadInfo_t& q = currentLine->quads.Last();
			q.origin = pen + Vector2((float)gi->offsetLeft + kerning, -(float)gi->offsetTop);
			q.width = (float)gi->rect.width;
			q.height = (float)gi->rect.height;
			q.texCoord = texR;
			q.transposed = gi->transposed;
			q.color = color->ToColor255();
			q.color2 = Color255(0, 255, 0, 0);

			const Vector2 q2 = q.origin;
			Vector2::Min(&currentLine->boundsTopLeft, &q2, &currentLine->boundsTopLeft);
			Vector2 tmp = Vector2(q.origin.x + q.width, q.origin.y + q.height);
			Vector2::Max(&currentLine->boundsBottomRight, &tmp, &currentLine->boundsBottomRight);

			currentLine->boundsTopLeft.y = pen.y;
			currentLine->boundsBottomRight.y = pen.y + stdLineHeight;

			Vector2::Min(&boundsMin, &currentLine->boundsTopLeft, &boundsMin);
			Vector2::Max(&boundsMax, &currentLine->boundsBottomRight, &boundsMax);
		}

		pen.x += (float)gi->advanceX * charSpacing;

		prev = gi;
	}

	preferredSize = boundsMax - boundsMin;
}

TextTrueType::LineInfo_t* TextTrueType::NewLineInfo()
{
	lines.IncreaseSize(1);
	if (lines.Last() == NULL)
		lines.Last() = new LineInfo_t();
	else
		lines.Last()->Reset();
	return lines.Last();
}