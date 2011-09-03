#ifndef TDATETIMEUNIX_H
#define TDATETIMEUNIX_H

#include "raisetypes.h"

class TDateTime;
class TDateTimeExpanded;

/**
 * Holds data as second precision from 1.1.1970.
 */
class TDateTimeUnix
{
public:
	dword TimeStamp;

	TDateTimeUnix& operator = (const TDateTime& value);
	
	TDateTimeUnix& operator = (const TDateTimeExpanded& value);

	TDateTimeUnix& operator = (dword value)
	{
		TimeStamp = value;
	}

	TDateTimeUnix(dword value)
	{
		TimeStamp = value;
	}
};


#endif