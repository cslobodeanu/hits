#ifndef __MORGANA_FMK_CANVAS_DOCUMENT_RENDER_H__
#define __MORGANA_FMK_CANVAS_DOCUMENT_RENDER_H__

#include "reflection.h"
#include "metypes.h"
#include "renderstocanvas.h"
#include "../resources/fonts/fonts.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			class DocumentRender : public RendersToCanvas
			{
				__declare_class(DocumentRender, RendersToCanvas);
			public:

				DocumentRender();
				virtual ~DocumentRender();

			protected:

				enum SpecialWordCodes
				{
					Default = 0,
					ArrowLeft,
					ArrowRight,
					ArrowUp,
					ArrowDown,
					Tab,
					NewLine
				};

				struct WordInfo_t
				{
					uint			strOffset;
					uint			strLength;

					Vector2			pen;
					Vector2			advance;
					float			angle;

					Color			color;
					Color			bkgColor;

					FontTrueType*	font;
					byte			fontSize;

					ushort			special;
					uint			specialData;

					void*			userData;
				};

				Array<WordInfo_t>	words;
				Array<int>			wordsToRender;

				virtual void		OnPreRenderObject();
				virtual void		OnRenderObject();
				virtual void		OnPostRenderObject();
			};
		}
	}
}

#endif