#include "time.h"
#include "meclock.h"

using namespace MorganaEngine::Framework::Times;

float			Time::deltaTime = 0.0f;
float			Time::time = 0.0f;
double			Time::timeOfStart = 0.0;
double			Time::lastFrame = 0.0;
unsigned long	Time::numFrames = 0;
float			Time::fixedFrameRate = 0.0f;

void Time::Reset()
{
	const double now = MESystemClock::Now();
	timeOfStart = now;
	lastFrame = now;
}

void Time::Tick()
{
	const double now = MESystemClock::Now();

	time = (float)(now - timeOfStart);
	deltaTime = (float)(now - lastFrame);
	
	lastFrame = now;

	numFrames++;
	DelayForFixedFramerate();
}

void Time::DelayForFixedFramerate()
{
	if (fixedFrameRate <= 0.0f) return;

	float dt = 1.0f / fixedFrameRate;
	if (deltaTime > dt) return;

	WaitForSeconds(dt - deltaTime);
}

void Time::WaitForSeconds(const float s)
{
	double now = MESystemClock::Now();
	const double start = now;
	while ((now - start) < (double)s)
	{
		MEApplication::main->CallApplicationWait();
		now = MESystemClock::Now();
	}
}
