#ifndef __MORGANA_BASE_TIME_SYSTEM_CLOCK_H__
#define __MORGANA_BASE_TIME_SYSTEM_CLOCK_H__

namespace MorganaEngine
{
	namespace Base
	{
		namespace Clock
		{
			/*
				High resolution timer class.
			*/
			class MESystemClock
			{
				static unsigned long	m_iFirstTick;
				static unsigned long	GetClockValueMS();
			public:
				static double			Now();
			};
		}
	}
}


#endif






