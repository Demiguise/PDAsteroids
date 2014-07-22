#pragma once
#include "Common.h"

class ScopedTimer
{
public:
	ScopedTimer(const char* name);
	~ScopedTimer();
private:
	float secsPerCount;
	UINT64 startCount;
	char* timerName;
};

