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
	static TString UniversalSortableDateTimePattern;

	TString DayNames[7];
	TString AbbreviatedDayNames[7];
	TString ShortestDayNames[7];

	TString MonthNames[12];
	TString AbbreviatedMonthNames[12];


	TString AMDesignator;
	TString PMDesignator;

	TCalendarWeekRule WeekRule;
	TString DateSeparator;
	TString TimeSeparator;

	TString FullDateTimePattern;
	TString LongDatePattern;
	TString LongTimePattern;
	TString MonthDayPattern;
	TString ShortDatePattern;
	TString ShortTimePattern;
	TString YearMonthPatter;
};

#endif