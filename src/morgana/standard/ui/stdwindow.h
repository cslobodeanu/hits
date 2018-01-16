#ifndef __MORGANA_STANDARD_UI_STANDARD_WINDOW_H__
#define __MORGANA_STANDARD_UI_STANDARD_WINDOW_H__

#include "framework.h"

namespace MorganaEngine
{
	namespace Standard
	{
		namespace UI
		{
			class StdWindow : public Window
			{
				__declare_class(StdWindow, Window);
			public:
				StdWindow();

				virtual void Awake();

				virtual void OnRenderObject();

			protected:

				virtual void Start();
				virtual void Update();
			};
		}
	}
}

#endif