#ifndef __MORGANA_STANDARD_DISPLAY_FPS_H__
#define __MORGANA_STANDARD_DISPLAY_FPS_H__

#include "framework.h"

namespace MorganaEngine
{
	namespace Standard
	{
		class DisplayFPS : public MEScript
		{
			__declare_morgana_script1(DisplayFPS);
		public:

			DisplayFPS();
			virtual ~DisplayFPS();

		protected:

			virtual void Start();
			virtual void Update();

			RectTransform*	myRectTransform;

			TextTrueType*	caption[10];

			float			averageFPS, cumulatedFrameTime;

			void			OnResize();
		};
	}
}

#endif