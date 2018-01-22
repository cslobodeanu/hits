#ifndef __MORGANA_FRAMEWORK_CANVAS_TEXT_TRUE_TYPE_H__
#define __MORGANA_FRAMEWORK_CANVAS_TEXT_TRUE_TYPE_H__

#include "../resources/fonts/fonts.h"
#include "renderstocanvas.h"
#include "recttransform.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			enum TextHorizontalAlignment
			{
				AlignLeft,
				AlignMiddle,
				AlignRight
			};

			enum TextVerticalAlignment
			{
				AlignTop,
				AlignCenter,
				AlignBottom
			};

			class TextTrueType : public RendersToCanvas
			{
				__declare_component(TextTrueType, RendersToCanvas);
			public:

				TextTrueType();
				virtual ~TextTrueType();

				_property<String>	text;
				_property<int>		fontSize;
				_property<float>	charSpacing;
				_property<float>	lineSpacing;
				_property<bool>		overflow;
				_property<bool>		fontItalic;
				_property<int>		alignment;
				_property<int>		verticalAlignment;

				_property_object<FontTrueType> font;

				const virtual Vector2 GetPreferredSize() const;

				template<class charType>
				static int			Draw(RendersToCanvas* rc, FontTrueType* ttf, const int fontSize, const charType* txt, const Vector2& pos, Material* mat, void** cache = NULL);

				template<class charType>
				static Vector2		Measure(RendersToCanvas* rc, FontTrueType* ttf, const int fontSize, const charType* txt);

			protected:

				virtual void Start();
				virtual void Update();

				Vector2		preferredSize;

				virtual void OnPreRenderObject();
				virtual void OnRenderObject();
				virtual void OnPostRenderObject();

				virtual void OnPropertySet(_propertyDef* atr);
				virtual void DoAddProperties();

				const void	NewLine(Vector2& pen);

				virtual void OnResizeRect();

				struct LineInfo_t
				{
					String				chars;
					Array<QuadInfo_t>	quads;
					Vector2				boundsTopLeft, boundsBottomRight;

					Array<CanvasSurface::VertexFormat> cachedQuadVertices;

					LineInfo_t()
					{
						Reset();
					}

					void Reset()
					{
						chars = "";
						quads.Clear();
						boundsTopLeft = Vector2::One * Mathf::MaxFloat;
						boundsBottomRight = -boundsTopLeft;
						cachedQuadVertices.Free();
					}
				};

				Array<LineInfo_t*> lines;
				LineInfo_t* NewLineInfo();

				void		ComputeLines();

				bool		mustUpdateQuads;

				bool		isBigText;
			};

			template<class charType>
			int TextTrueType::Draw(RendersToCanvas* rc, FontTrueType* ttf, const int fontSize, const charType* txt, const Vector2& pos, Material* mat, void** cache /*= NULL*/)
			{
				if (ttf == NULL) return 0;
				if (txt == NULL || txt[0] == 0) return 0;

				static Array<QuadInfo_t> quads;
				quads.Clear();

				const float stdLineHeight = ttf->GetLineHeight(fontSize);

				Vector2 pen = Vector2::Zero;
				pen.y = stdLineHeight;

				FontTrueType::GlyphInfo_t* gi = NULL;
				FontTrueType::GlyphInfo_t* prev = NULL;

				FontTrueType::GlyphInfo_t spaceGlyph;
				ttf->GetGlyphInfo(' ', fontSize, spaceGlyph);

				Texture* texToUse = NULL;

				for (const charType* ch = txt; *ch; ch++)
				{
					gi = ttf->RequestGlyph(*ch, fontSize);
					if (gi == NULL) continue;

					texToUse = gi->texture;

					if (gi->rect.width > 0)
					{
						Rectf texR((float)gi->rect.x, (float)gi->rect.y, (float)gi->rect.width, (float)gi->rect.height);
						texR.Normalize((float)gi->texture->GetBounds().width);

						const float kerning = ttf->GetKerning(gi, prev);

						quads.IncreaseSize(1);

						QuadInfo_t& q = quads.Last();
						q.origin = pen + Vector2((float)gi->offsetLeft + kerning, -(float)gi->offsetTop);
						q.width = (float)gi->rect.width;
						q.height = (float)gi->rect.height;
						q.texCoord = texR;
						q.transposed = gi->transposed;
						q.color = rc->GetSpriteColor();
						q.color2 = Color255(0, 255, 0, 0);
					}

					pen.x += (float)gi->advanceX;

					prev = gi;
				}

				if (rc != NULL && gi != NULL)
				{
					if (cache == NULL)
						mat->SetTexture(texToUse);
					rc->PushQuadsM(quads.ptr(), quads.Length(), pos, cache, mat);
				}

				return quads.Length();
			}

			template<class charType>
			Vector2	TextTrueType::Measure(RendersToCanvas* rc, FontTrueType* ttf, const int fontSize, const charType* txt)
			{
				if (ttf == NULL) return Vector2::Zero;
				if (txt == NULL || txt[0] == 0) return Vector2::Zero;

				Vector2 pen = Vector2::Zero;
				pen.y = ttf->GetLineHeight(fontSize);

				FontTrueType::GlyphInfo_t* gi = NULL;
				FontTrueType::GlyphInfo_t* prev = NULL;

				Vector2 boundsMin = Vector2::One * Mathf::MaxFloat;
				Vector2 boundsMax = -boundsMin;

				const float stdLineHeight = ttf->GetLineHeight(fontSize);

				FontTrueType::GlyphInfo_t spaceGlyph;
				ttf->GetGlyphInfo(' ', fontSize, spaceGlyph);

				for (const charType* ch = txt; *ch; ch++)
				{
					gi = ttf->RequestGlyph(*ch, fontSize);
					if (gi == NULL) continue;

					if (gi->rect.width > 0)
					{
						const float kerning = ttf->GetKerning(gi, prev);

						const Vector2 pos = pen + Vector2((float)gi->offsetLeft + kerning, -(float)gi->offsetTop);
						Vector2::Min(&boundsMin, &pos, &boundsMin);
						const Vector2 tmp = Vector2(pos.x + (float)gi->rect.width, pos.y + (float)gi->rect.height);
						Vector2::Max(&boundsMax, &tmp, &boundsMax);
					}

					pen.x += (float)gi->advanceX;

					prev = gi;
				}

				return boundsMax - boundsMin;
			}
		}
	}
}


#endif