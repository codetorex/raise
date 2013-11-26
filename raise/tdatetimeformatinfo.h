#ifndef TDATETIMEFORMATINFO_H
#define TDATETIMEFORMATINFO_H

#include "tstring.h"

enum TCalendarWeekRule
{
	CWR_FIRSTDAY,
	CWR_FIRSTFULLWEEK,
	CWR_FIRSTFOURDAYWEEK,
};

class TDateTimeFormatInfo
{
public:
	static String UniversalSortableDateTimePattern;

	String DayNames[7];
	String AbbreviatedDayNames[7];
	String ShortestDayNames[7];

	String MonthNames[12];
	String AbbreviatedMonthNames[12];


	String AMDesignator;
	String PMDesignator;

	TCalendarWeekRule WeekRule;
	String DateSeparator;
	String TimeSeparator;

	String FullDateTimePattern;
	String LongDatePattern;
	String LongTimePattern;
	String MonthDayPattern;
	String ShortDatePattern;
	String ShortTimePattern;
	String YearMonthPatter;
};

#endif