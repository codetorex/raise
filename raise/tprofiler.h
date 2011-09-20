#ifndef TPROFILER_H
#define TPROFILER_H

#include "m64bit.h"
#include "ttimedriver.h"

class TProfiler
{
private:
	ui32 tickStart;

public:
	ui32 callCount;
	ui32 tickDelta;

	inline void BeginProfiling()
	{
		tickStart = TimeDriver::TickCount();
	}

	inline void EndProfiling()
	{
		tickDelta = TimeDriver::TickCount() - tickStart;
	}

	inline void BeginProfiling(ui32 callCount)
	{
		this->callCount = callCount;
		BeginProfiling();
	}

	/**
	* Returns per operation time as nanosecond.
	*/
	inline double PerOperationTime()
	{
		double ns = (double)tickDelta / (double)callCount;
		ns *= (double)1000000.0f; // millisecond, microsecond, nanosecond 1million for micro and nano
		return ns;
	}
};





#endif