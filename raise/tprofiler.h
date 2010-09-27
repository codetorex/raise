#ifndef TPROFILER_H
#define TPROFILER_H

#include "m64bit.h"
#include "ttiming.h"



class TProfiler
{
private:
	dword tickStart;

public:
	dword callCount;
	dword tickDelta;

	void BeginProfiling()
	{
		tickStart = TickCount();
	}

	void EndProfiling()
	{
		tickDelta = TickCount() - tickStart;
	}

	void BeginProfiling(dword callCount)
	{
		this->callCount = callCount;
		BeginProfiling();
	}
};


#endif