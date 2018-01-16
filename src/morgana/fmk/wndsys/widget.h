#ifndef __MORGANA_FRAMEWORK_WNDSYS_WIDGET_H__
#define __MORGANA_FRAMEWORK_WNDSYS_WIDGET_H__

#include "gameobjects.h"
#include "../canvas/renderstocanvas.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Canvas;
		namespace WindowSystem
		{
			class Widget : public RendersToCanvas
			{
				__declare_component(Widget, RendersToCanvas);
			public:
				Widget();
				virtual ~Widget();

				Action<Widget*, Vector2, bool&> OnTouchDownEvent;
				Action<Widget*, Vector2, bool&> OnTouchUpEvent;
				Action<Widget*, Vector2, bool&> OnTouchMoveEvent;
				Action<Widget*, Vector2, bool&> OnTouchClickEvent;

				_property<String>	caption;

				virtual void OnPreRenderObject();
				virtual void OnRenderObject();
				virtual void OnPostRenderObject();

				static Widget*	capture;

			protected:
				virtual void Awake();
				virtual void Start();
				virtual void Update();

				virtual void DoAddProperties();

				virtual void OnTouchDown(Component*, Vector2 p, bool& handled);
				virtual void OnTouchUp(Component*, Vector2 p, bool& handled);
				virtual void OnTouchMove(Component*, Vector2 p, bool& handled);
				virtual void OnTouchClick(Component*, Vector2 p, bool& handled);

			private:

				void _OnTouchDown(Component*, Vector2 p, bool& handled);
				void _OnTouchUp(Component*, Vector2 p, bool& handled);
				void _OnTouchMove(Component*, Vector2 p, bool& handled);
				void _OnTouchClick (Component*, Vector2 p, bool& handled);
			};
		}
	}
}

#endif