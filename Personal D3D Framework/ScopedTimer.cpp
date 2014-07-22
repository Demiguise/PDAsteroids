#include "ScopedTimer.h"
#include "GameLog.h"

ScopedTimer::ScopedTimer(const char* name)
{
	UINT64 countsPerSecond = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	secsPerCount = 1.0f / (float)countsPerSecond;
	QueryPerformanceCounter((LARGE_INTEGER*)&startCount);
	memcpy(&timerName, &name, sizeof(char*));
}


ScopedTimer::~ScopedTimer()
{
	UINT64 tempCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&tempCount);
	float elapsedTime = ((tempCount - startCount) * secsPerCount);
	GameLog::GetInstance()->Log(DebugChannel::All, DebugLevel::None, "[Timers] Timer for %s took %f seconds to complete", timerName, elapsedTime);
}
