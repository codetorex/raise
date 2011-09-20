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
	ui32 TimeStamp;

	TDateTimeUnix& operator = (const TDateTime& value);
	
	TDateTimeUnix& operator = (const TDateTimeExpanded& value);

	TDateTimeUnix& operator = (ui32 value)
	{
		TimeStamp = value;
	}

	TDateTimeUnix(ui32 value)
	{
		TimeStamp = value;
	}
};


#endif