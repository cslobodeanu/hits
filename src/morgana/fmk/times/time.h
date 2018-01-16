#ifndef __MORGANA_FMK_TIME2_TIME2_H__
#define __MORGANA_FMK_TIME2_TIME2_H__

#include "../application/meapplication.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Application;
		namespace Times
		{
			class Time
			{
			public:

				static float			deltaTime;
				static float			time;

				static unsigned long	numFrames;

				static float			fixedFrameRate;

				friend class MEApplication;

				static void				WaitForSeconds(const float s);

			protected:
				static void				Reset();
				static void				Tick();

			private:

				static void				DelayForFixedFramerate();

				static double timeOfStart;
				static double lastFrame;
			};
		}
	}
}

#endif