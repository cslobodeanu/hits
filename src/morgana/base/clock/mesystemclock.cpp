#include "mesystemclock.h"
#include "platform.h"

#ifdef OS_WINDOWS
#include <windows.h>
#elif defined OS_APPLE_IOS
#include "mach/mach_time.h"
#include <sys/time.h>
#endif

#include <ctime>

#define MILLIONS	1000000L

using namespace MorganaEngine::Base::Clock;

unsigned long MESystemClock::m_iFirstTick = GetClockValueMS();
time_t startMoment = time(0);

inline double __TicksPerSecond()
{
#ifdef OS_WINDOWS
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency(&ticksPerSecond);
	return (double)ticksPerSecond.QuadPart;
#else
	return (double)MILLIONS;
#endif
}

unsigned long MESystemClock::GetClockValueMS()
{
#ifdef OS_WINDOWS
    LARGE_INTEGER tick;
    QueryPerformanceCounter(&tick);

	const double multiplier = 1000.0f;
	const double ms = (tick.QuadPart * multiplier) / __TicksPerSecond();
	return (unsigned long)ms + (unsigned long)startMoment * 1000;
#elif defined OS_APPLE_IOS
	timeval time;
    gettimeofday(&time, NULL);
	if(nano)
		return (time.tv_sec * MILLIONS) + time.tv_usec;
	else
		return (time.tv_sec * 1000) + (time.tv_usec / 1000);
#endif
}

double MESystemClock::Now()
{
	return (double)(GetClockValueMS() - m_iFirstTick) / 1000.0;
}
