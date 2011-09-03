
#ifndef TEST_DATETIME_H
#define TEST_DATETIME_H

#include "ttest.h"
#include "tdatetime.h"

class TestTDateTime: public TTestCheck
{
public:
	TestTDateTime(): TTestCheck("TDateTime check"){}

	void Test()
	{
		TDateTime tdate(2011,07,12,5,6,7,8);
		TDateTimeExpanded edate = tdate;

		AddResult(edate.Year == 2011, "Year expansion");
		AddResult(edate.Month == 7, "Month expansion");
		AddResult(edate.Day == 12, "Day expansion");
		AddResult(edate.Hour == 5, "Hour expansion");
		AddResult(edate.Minute == 6, "Minute expansion");
		AddResult(edate.Second == 7, "Second expansion");
		AddResult(edate.MilliSecond == 8, "Millisecond expansion");
		AddResult(edate.DayOfWeek == 1, "Day of week expansion");

		tdate.AddDays(5);
		edate = tdate;

		AddResult(edate.Day == 17,"Add 5 days");
		AddResult(edate.DayOfWeek == 6, "Day of week after addition");

		TDateTime leap(2012,02,29);
		edate = leap;
		AddResult(edate.Leap, "2012 is leap year");
		AddResult(edate.Day == 29 && edate.Month == 2, "Leap day check");

		leap.SetDate(2012,02,28);
		edate = leap;
		AddResult(edate.Day == 28 && edate.Month == 2, "One day before leap day");

		leap.SetDate(2012,03,01); // I know feb doesnt have 30 days
		edate = leap;
		AddResult(edate.Day == 1 && edate.Month == 3, "One day after leap day");

		leap.SetDate(2012,01,01);
		edate = leap;
		AddResult(edate.Day == 1 && edate.Month == 1 && edate.Year == 2012, "First day of 2012");

		tdate.SetDate(2041,07,12); // 30 years after from starting to check this algorithm
		edate = tdate;
		AddResult(edate.Year == 2041 && edate.Month == 7 && edate.Day == 12, "Future date check");
		AddResult(edate.DayOfWeek == 4, "Future date, day of week check");
		AddResult(edate.DayOfYear == 193, "Future date, day of year check");

		AddResult(tdate.GetDaysFromAnchor() == 745287, "Future date, day count check GetDaysFromAnchor()");


		TDateTimeUnix tunix = 1310557123;
		edate = tunix;
		AddResult(edate.Year == 2011,"Unix date year");
		AddResult(edate.Month == 7,"Unix date month");
		AddResult(edate.Day == 13,"Unix date day");
		AddResult(edate.Hour == 11,"Unix date hour");
		AddResult(edate.Minute == 38,"Unix date minute");
		AddResult(edate.Second == 43,"Unix date second");

		tdate.SetDateTime(2011,7,13,11,38,43);
		tunix = tdate;
		AddResult(tunix.TimeStamp == 1310557123, "DateTime to Unix timestamp conversion");

		tdate = edate;
		tunix = tdate;
		AddResult(tunix.TimeStamp == 1310557123, "Expanded to DateTime to Unix timestamp conversion");

		tunix = edate;
		AddResult(tunix.TimeStamp == 1310557123, "Expanded to Unix timestamp conversion");

		edate.Set( TimeDriver::Now(), true);
		AddOutput("Local date time: " + edate.ToStringUniversal());
	}

} TDateTimeCheck;



#endif

