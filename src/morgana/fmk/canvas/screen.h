#ifndef __MORGANA_FMK_CANVAS_SCREEN_H__
#define __MORGANA_FMK_CANVAS_SCREEN_H__

#include "memath.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			class Screen
			{
			public:

				static int width;
				static int height;
				static int dpi;

				static Vector2 size;

				static void Update();

				static void Initialize();

			private:

				static void AquireSize();
			};
		}
	}
}

#endif