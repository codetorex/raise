#ifndef TDATETIMEWINDOWS_H
#define TDATETIMEWINDOWS_H

#include "raisetypes.h"
#include "m64bit.h"

#ifdef LINUX

typedef struct _FILETIME {
	ui32 dwLowDateTime;
	ui32 dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

#endif

/**
 * Class encapsulation for windows FILETIME structure.
 */
class TDateTimeWindows
{
public:
	FILETIME Date;

	TDateTimeWindows()
	{

	}

	TDateTimeWindows( FILETIME _date)
	{
		Date = _date;
	}

	// operator from tdatetime
	// operator to tdatetime
};

#endif