#ifndef TTIMEDRIVER_H
#define TTIMEDRIVER_H

#include "stdafx.h"
#include "raisetypes.h"
#include "m64bit.h"
#include "tdatetime.h"

#include "tutf16encoding.h"

#ifdef WIN32 // move this def outside of class?
class TimeDriver
{
public:
	static bool HighPrecisionSupported;

	static string LocalStandartName;
	static string LocalDaylightName;
	static int LocalDaylight;
	static ui32 LocalLastUpdate;

	/// Local time difference in seconds
	static int LocalDifference;

	static ui32 TickCount()
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

	static TDateTime Now()
	{
		TDateTimeWindows wintime;
		GetSystemTimeAsFileTime(&wintime.Date);
		
		TDateTime result(wintime);
		return result;
	}

	static TDateTime ToLocalTime(const TDateTime& utctime)
	{
		if (TickCount() - LocalLastUpdate > (60 * 60 * 1000)) // 1 hour
		{
			UpdateLocalTimezone(); // Do this every 2 hours or something
		}
		
		TDateTime result = utctime;
		result.AddSeconds(LocalDifference);
		return result;
	}

	static void UpdateLocalTimezone()
	{
		TIME_ZONE_INFORMATION dtz;
		ui32 status = GetTimeZoneInformation (&dtz);

		if (status == TIME_ZONE_ID_STANDARD || status == TIME_ZONE_ID_UNKNOWN)
		{
			LocalDaylight = 0;
		}
		else if (status == TIME_ZONE_ID_DAYLIGHT)
		{
			LocalDaylight = 1;
			dtz.Bias += dtz.DaylightBias;
		}

		LocalStandartName = TEncoding::UTF16.GetString(dtz.StandardName);
		LocalDaylightName = TEncoding::UTF16.GetString(dtz.DaylightName);
		LocalDifference = dtz.Bias * -60; // in seconds
	}
};

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