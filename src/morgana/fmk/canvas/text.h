#ifndef __MORGANA_FMK_CANVAS_TEXT_H__
#define __MORGANA_FMK_CANVAS_TEXT_H__

#include "../resources/fonts/fonts.h"
#include "renderstocanvas.h"
#include "recttransform.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			class Text : public RendersToCanvas
			{
				__declare_component(Text, RendersToCanvas);
			public:

				Text();
				virtual ~Text();

				enum
				{
					StyleSimple = 0,
					StyleShadowed = 1, // as bit
					StyleBackGlow = 2 // as bit
				};

				_property<int>		style;
				_property<Vector2>	scaling;

				_property_object<Font>		font;
				_property<String>			text;
				_property<bool>				wrap;

				virtual void Awake();

			protected:
				virtual void OnPreRenderObject();
				virtual void OnRenderObject();
				virtual void OnPostRenderObject();

				virtual void OnPropertySet(_propertyDef* atr);
				virtual void DoAddProperties();

			private:

				void	Draw(Font* f, const char* str, const float x, const float y);

				void	EventuallyCreateTextureForFont(Font* f);
				Vector2	GetCharSize(Font* f, const char ch);

				void	DrawBase(Font* f, const char* str, const float x, const float y);

				Vector2 Measure(Font* f, const char* str, int startChar = 0, int endChar = Mathf::MaxInt);
				float	GetLineHeight(Font* f);

				String	textToDraw;
				void	ConvertText();	

				char*	tempBuffer;
				int		tempBufferSize;
				char*	AllocTempBuffer(const int sz);

				RectTransform* myRectTransform;
			};
		}
	}
}

#endif