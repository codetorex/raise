#ifndef TPLATFORMUNIX_H
#define TPLATFORMUNIX_H

#ifdef LINUX

#include "tplatforminterface.h"
#include "tencoding.h"

#include <time.h>
#include <sys/time.h>

class TPlatformUnix: public IPlatform
{
public:

	/// THESE FUNCTIONS SHOULD BE REMOVED FROM FUCKING EXISTENCE
	void RaiseToSystemString( const String& raiseStr, byte* output, ui32 outputSize ) 
	{
		memcpy(output,raiseStr.Data,raiseStr.ByteLength);
	}

	String SystemToRaiseString( byte* input, ui32 inputSize ) 
	{
		String result(inputSize);
		memcpy(result.Data,input,inputSize);
	}

	ui32 TickCount() 
	{
		struct timespec now;
		if (clock_gettime(CLOCK_MONOTONIC, &now))
			return 0;
		double result = now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
		return (int)result;
	}

	TDateTime GetUtcTime() 
	{
		struct timeval tv;
		struct timezone tz;
		if(gettimeofday(&tv, &tz) != 0)
			return TDateTime(0);

		TDateTimeUnix unixdate(tv.tv_sec);
		TDateTime result;
		result = unixdate;
		result.AddTicks(tv.tv_usec);

		//result.AddMinutes(-tz.tz_minuteswest);

		return result;
	}

	TDateTime ToLocalTime( const TDateTime& inputDateTime ) 
	{
		tzset();

		/*struct timezone tz;
		if(gettimeofday(NULL, &tz) != 0)
			return TDateTime(0);*/

		TDateTime result = inputDateTime;
		result.AddSeconds(-timezone);
		return result;
	}

	qword HighPrecisionTime() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	qword HighPrecisionTimeFrequency() 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	void ShowMessageBox( const String& message ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	String GetErrorDescription( ui32 systemErrorID ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	bool BrowseFolder( const String& startPath, String& selectedPath ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

};


typedef TPlatformUnix TPlatformCurrent;

#endif // UNIX



#endif