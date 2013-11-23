
#ifndef TEST_DATETIME_H
#define TEST_DATETIME_H

#include "ttest.h"
#include "tdatetime.h"

class TestTDateTime: public UnitTest
{
public:
	TestTDateTime(): UnitTest("TDateTime check"){}

	void Execute()
	{
		TDateTime tdate(2011,07,12,5,6,7,8);
		TDateTimeExpanded edate = tdate;

		Result(edate.Year == 2011, "Year expansion");
		Result(edate.Month == 7, "Month expansion");
		Result(edate.Day == 12, "Day expansion");
		Result(edate.Hour == 5, "Hour expansion");
		Result(edate.Minute == 6, "Minute expansion");
		Result(edate.Second == 7, "Second expansion");
		Result(edate.MilliSecond == 8, "Millisecond expansion");
		Result(edate.DayOfWeek == 1, "Day of week expansion");

		tdate.AddDays(5);
		edate = tdate;

		Result(edate.Day == 17,"Add 5 days");
		Result(edate.DayOfWeek == 6, "Day of week after addition");

		TDateTime leap(2012,02,29);
		edate = leap;
		Result(edate.Leap, "2012 is leap year");
		Result(edate.Day == 29 && edate.Month == 2, "Leap day check");

		leap.SetDate(2012,02,28);
		edate = leap;
		Result(edate.Day == 28 && edate.Month == 2, "One day before leap day");

		leap.SetDate(2012,03,01); // I know feb doesnt have 30 days
		edate = leap;
		Result(edate.Day == 1 && edate.Month == 3, "One day after leap day");

		leap.SetDate(2012,01,01);
		edate = leap;
		Result(edate.Day == 1 && edate.Month == 1 && edate.Year == 2012, "First day of 2012");

		tdate.SetDate(2041,07,12); // 30 years after from starting to check this algorithm
		edate = tdate;
		Result(edate.Year == 2041 && edate.Month == 7 && edate.Day == 12, "Future date check");
		Result(edate.DayOfWeek == 4, "Future date, day of week check");
		Result(edate.DayOfYear == 193, "Future date, day of year check");

		Result(tdate.GetDaysFromAnchor() == 745287, "Future date, day count check GetDaysFromAnchor()");


		TDateTimeUnix tunix = 1310557123;
		edate = tunix;
		Result(edate.Year == 2011,"Unix date year");
		Result(edate.Month == 7,"Unix date month");
		Result(edate.Day == 13,"Unix date day");
		Result(edate.Hour == 11,"Unix date hour");
		Result(edate.Minute == 38,"Unix date minute");
		Result(edate.Second == 43,"Unix date second");

		tdate.SetDateTime(2011,7,13,11,38,43);
		tunix = tdate;
		Result(tunix.TimeStamp == 1310557123, "DateTime to Unix timestamp conversion");

		tdate = edate;
		tunix = tdate;
		Result(tunix.TimeStamp == 1310557123, "Expanded to DateTime to Unix timestamp conversion");

		tunix = edate;
		Result(tunix.TimeStamp == 1310557123, "Expanded to Unix timestamp conversion");

		edate.Set( Platform.GetUtcTime(), true);
		Output("Local date time: " + edate.ToStringUniversal());
	}

} TDateTimeCheck;



#endif

