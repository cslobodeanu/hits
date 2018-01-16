#ifndef __MORGANA_FRAMEWORK_WNDSYS_WINDOW_H__
#define __MORGANA_FRAMEWORK_WNDSYS_WINDOW_H__

#include "widget.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace WindowSystem
		{
			class Window : public Widget
			{
				__declare_class(Window, Widget);
			public:

				Window();
				virtual ~Window();

				_property<bool>		sizeable;
				_property<bool>		movable;
				_property<Vector2>	minSize;
				_property<Vector2>	maxSize;

				static Window*	currentActive;

				void		Activate();

				Rectf				clientRect;

			protected:

				virtual void DoAddProperties();

				virtual void Start();
				virtual void Update();

				virtual void OnTouchDown(Component*, Vector2 p, bool& handled);
				virtual void OnTouchUp(Component*, Vector2 p, bool& handled);
				virtual void OnTouchMove(Component*, Vector2 p, bool& handled);
			};
		}
	}
}

#endif