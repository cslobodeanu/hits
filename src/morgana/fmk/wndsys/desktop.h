#ifndef __MORGANA_FRAMEWORK_WNDSYS_DESKTOP_H__
#define __MORGANA_FRAMEWORK_WNDSYS_DESKTOP_H__

#include "widget.h"
#include "../canvas/recttransform.h"
#include "../canvas/canvassurface.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Canvas;
		namespace WindowSystem
		{
			class DesktopTheme;
			class Desktop : public Widget
			{
				__declare_class(Desktop, Widget);
			public:
				Desktop();
				virtual ~Desktop();

				static Desktop* main;

				DesktopTheme*	theme;

				virtual void Awake();

			protected:
				virtual void Start();
				virtual void Update();

				RectTransform*	rectTransform;
				CanvasSurface*	canvas;

				virtual void OnParentAttached();
			};
		}
	}
}

#endif