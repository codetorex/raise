#ifndef TTIMEDRIVER_H
#define TTIMEDRIVER_H

#include "stdafx.h"
#include "raisetypes.h"
#include "m64bit.h"

#ifdef WIN32 // move this def outside of class?
class TTimeDriver
{
public:
	static bool HighPrecisionSupported;


	static dword TickCount()
	{
		return GetTickCount();
	}

	static qword HighPrecision()
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
	}
};

#else
#include <sys/times.h>

class TTimeDriver
{
public:
	static bool HighPrecisionSupported;


	static dword TickCount()
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