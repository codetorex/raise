#ifndef TTIMEDRIVER_H
#define TTIMEDRIVER_H

#include "stdafx.h"
#include "raisetypes.h"
#include "m64bit.h"
#include "tdatetime.h"



#ifdef WIN32 // move this def outside of class?

#include "tplatform.h"



#else
#include <sys/times.h>
#include <sys/time.h>

class TimeDriver
{
public:
	static bool HighPrecisionSupported;

	// http://linux.die.net/man/3/daylight

	static ui32 TickCount()
	{
		tms tm;
		return times(&tm);
	}

	/*static qword HighPrecision()
	{
		qword val;
		if ( !QueryPerformanceCounter((LARGE_INTEGER*)&val) )
		{
			HighPrecisionSupported = false;
			return TickCount();
		}

		return val;
	}

	static qword HighFrequency()
	{
		if (!HighPrecisionSupported)
			return 1000;

		qword val;
		QueryPerformanceFrequency((LARGE_INTEGER*)&val);
		return val;
	}*/



};

#endif

#endif