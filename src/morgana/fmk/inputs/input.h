#ifndef __MORGANA_FMK_INPUTS_INPUT_H__
#define __MORGANA_FMK_INPUTS_INPUT_H__

#include "base.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Inputs
		{
			class Input
			{
				static InputDevice*	kb;
				static InputDevice*	mouse;
				static void			Initialize();

				static void			OnDeviceLost(MEDevice* dev);
			public:

				static bool			GetKeyDown(int key);
				static bool			GetKeyPressed(int key);
				static bool			GetKeyReleased(int key);
				static Vector2		GetMousePos();
				static Vector2		GetMousePosScr();
				static bool			GetMouseButtonDown(int button);
				static bool			GetMouseButtonPressed(int button);
				static bool			GetMouseButtonReleased(int button);
				static int			GetMouseWheel();
			};
		}
	}
}


#endif