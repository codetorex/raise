#ifndef TDATETIME_H
#define TDATETIME_H

#include <time.h>

class TDateTimeExpanded
{
public:
/*	TDateTimeExpanded(const TDateTime& time)
	{

	}*/

	int second;
	int minute;
	int hour;
	int month;
	int year;
};

class TDateTime
{
public:
	time_t mtime;


/*	word year;
	byte month;
	byte dayofweek;
	byte day;
	byte hour;
	byte minute;
	byte second;
	word milliseconds;*/
};


#endif