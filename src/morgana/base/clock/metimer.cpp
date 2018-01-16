#include "metimer.h"
#include "mesystemclock.h"

using namespace MorganaEngine::Base::Clock;

__implement_class(METimer, MEObject);

Array<METimer*>	METimer::allTimers;

METimer::METimer()
{
	deltaTime = 1.0f;
	allowedTicks = 1;
	tickCount = 0;
	
	Resume();

	allTimers.Add(this);
}

METimer::METimer(const float dt, const int count /*= -1*/)
{
	deltaTime = dt;
	allowedTicks = count;
	tickCount = 0;
	
	allTimers.Add(this);

	Resume();
}

METimer::~METimer()
{
	allTimers.Remove(this);
}

void METimer::Pause()
{
	isRunning = false;
}

void METimer::Resume()
{
	isRunning = true;
	lastTime = MESystemClock::Now();
}

void METimer::UpdateAll()
{
	METimer** ptr = allTimers.ptr();
	METimer** end = allTimers.end();

	for(;ptr < end; ptr++)
	{
		(*ptr)->Update();
	}
}

void METimer::Destroy()
{
	for(int i = 0; i < allTimers.Length(); i++)
	{
		METimer* last = allTimers[i];
		SAFE_DEL(last);
	}
}

void METimer::Update()
{
	if(allowedTicks > 0 && tickCount > allowedTicks) return;

	const double now = MESystemClock::Now();
	const float delta = (float)(now - lastTime);
	if(delta < deltaTime) return;

	lastTime = now;

	OnTick(this, delta / deltaTime);
	tickCount++;
}

METimer* METimer::FindTimerByName(const char* name)
{
	const int hash = String::ComputeHashCode(name);
	for(int i = 0; i < allTimers.Length(); i++)
	{
		if(allTimers[i]->name.Equals(name, hash))
			return allTimers[i];
	}

	return NULL;
}
