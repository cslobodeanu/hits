#ifndef __MORGANA_BASE_CLOCK_METIMER_H__
#define __MORGANA_BASE_CLOCK_METIMER_H__

#include "reflection.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Clock
		{
			class METimer : public MEObject
			{
				__declare_class(METimer, MEObject);
				METimer();

				int		allowedTicks, tickCount;
				float	deltaTime;
				bool	isRunning;
				double	lastTime;

				static Array<METimer*>	allTimers;

				void	Update();

				StringHash	name;

			public:
				METimer(const float dt, const int count = -1);
				virtual ~METimer();

				void	Pause();
				void	Resume();

				inline const void SetName(const char* name) { name = name; }
				inline const String& GetName() const { return name; }

				static void UpdateAll();
				static void Destroy();

				static METimer* FindTimerByName(const char* name);

				Action<METimer*, float>		OnTick;
			};
		}
	}
}

#endif