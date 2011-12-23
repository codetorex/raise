#ifndef TDATETIMEWINDOWS_H
#define TDATETIMEWINDOWS_H

#include "raisetypes.h"
#include "m64bit.h"

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