#ifndef __MORGANA_BASE_CLOCK_DATE_TIME_H__
#define __MORGANA_BASE_CLOCK_DATE_TIME_H__

namespace MorganaEngine
{
	namespace Base
	{
		namespace Clock
		{
			typedef struct
			{
				char		day, month, hour, minute, sec;
				short		year;
			} DateTime_t;
		}
	}
}

#endif