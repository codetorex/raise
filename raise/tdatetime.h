#ifndef TDATETIME_H
#define TDATETIME_H

#include "raisetypes.h"

#include "tdatetimewindows.h"
#include "tdatetimeunix.h"
#include "tdatetimeexpanded.h"
#include "mmathdriver.h"

class TDateTimeExpanded;



/**
 * Holds data as int64 by microsecond precision from 1.1.1 and can be negative to represent older dates.
 */
class TDateTime
{
public:
	/// Represents numbers of microseconds since 1 Jan, AD 1
	int64 Ticks;

	static const int MonthDays[];
	static const int MonthStartDay[];
	static const int MonthStartLeap[];

	/**
	 * Returns number of leap days in total day count.
	 */
	inline static int GetLeapDays(int totaldays)
	{
		return  (totaldays / 1461) - (totaldays / 36524) + (totaldays / 146097);
	}

	/**
	 * Returns number of leap years in total year count.
	 */
	inline static int GetLeapYears(int totalyears)
	{
		return (totalyears / 4) - (totalyears / 100) + (totalyears / 400);
	}

	/**
	 * Returns true if given year is a leap year.
	 */
	inline static bool IsLeapYear(int year)
	{
		if ( year%400 == 0 || (year%100 != 0 && year % 4 == 0))
		{
			return true;
		}
		return false;
	}

	/**
	 * Sets date that given in parameters.
	 * Note: this function will resets time data.
	 */
	void SetDate(int year,int month,int day);

	/**
	 * Sets date that given in parameters without affecting time data.
	 */
	void ChangeDate(int year,int month,int day)
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	/**
	 * Adds specified times in parameters.
	 */
	void AddTime(int hours,int minutes, int seconds,int milliseconds,int microseconds);

	/**
	 * Resets this object to new date and time.
	 */
	inline void SetDateTime(int year, int month, int day,int hour, int minute,int second,int milliseconds = 0, int microseconds = 0)
	{
		SetDate(year,month,day);
		AddTime(hour,minute,second,milliseconds,microseconds);
	}

	TDateTime& operator = (const TDateTimeUnix& value);

	inline TDateTime& operator = (const TDateTimeExpanded& value)
	{
		SetDateTime(value.Year,value.Month,value.Day,value.Hour,value.Minute,value.Second,value.MilliSecond,value.MicroSecond);
		return *this;
	}

	TDateTime& operator = (const TDateTimeWindows& value)
	{
		int64 systime = (((int64)value.Date.dwHighDateTime) << 32) | value.Date.dwLowDateTime;
		systime /= 10; // since it is 100 nanosecond precision, we use microsecond precision
		Ticks = systime;
		AddDays(584388); // Days between 1AD to 1601 AD
		return *this;
	}

	inline TDateTime& operator = (const TDateTime& value)
	{
		Ticks = value.Ticks;
		return *this;
	}

	inline TDateTimeExpanded Expand() const
	{
		TDateTimeExpanded te = *this;
		return te;
	}

	TDateTime()
	{
		Ticks = 0;
	}

	TDateTime(int64 _ticks)
	{
		Ticks = _ticks;
	}

	TDateTime(int year,int month,int day)
	{
		SetDate(year,month,day);
	}

	TDateTime(int year,int month,int day ,int hour, int minute, int second, int millisecond = 0,int microsecond = 0)
	{
		SetDateTime(year,month,day,hour,minute,second,millisecond,microsecond);
	}

	TDateTime(const TDateTimeUnix& value)
	{
		*this = value;
	}

	TDateTime(const TDateTimeExpanded& value)
	{
		*this = value;
	}

	TDateTime(const TDateTimeWindows& value)
	{
		*this = value;
	}

	/**
	 * Adds days to current date.
	 * @param days Signed integer day count to be added.
	 */
	inline TDateTime& AddDays(int days)
	{
		Ticks += (int64)days * 86400000000LL;
		return *this;
	}

	inline int GetDaysFromAnchor() const
	{
		return (int)(Ticks / 86400000000LL);
	}

	/**
	 * Adds days with real number representation to current date.
	 * @param days Floating point day count to be added. ( like 2.5, which adds 2 days and 12 hours )
	 */
	TDateTime& AddDaysReal(double days);

	inline TDateTime& AddMonths(int months)
	{
		throw NotImplementedException(__FILE__,__LINE__);
		return *this;
	}

	inline TDateTime& AddHours(int hours)
	{
		int64 addticks = hours;
		addticks *= 60LL * 60 * 1000000;
		Ticks += addticks;
		return *this;
	}

	inline TDateTime& AddMinutes(int minutes)
	{
		int64 addticks = minutes;
		addticks *= 60 * 1000000;
		Ticks += addticks;
		return *this;
	}

	inline TDateTime& AddSeconds(int seconds)
	{
		int64 addticks = seconds;
		addticks *= 1000000;
		Ticks += addticks;
		return *this;
	}

	inline TDateTime& AddYears(int years)
	{
		throw NotImplementedException(__FILE__,__LINE__);
		return *this;
	}

	inline TDateTime& AddMilliseconds(ui32 ms)
	{
		Ticks += ms * 1000;
		return *this;
	}

	inline TDateTime& AddTicks(int ticks)
	{
		Ticks += ticks;
	}
};

typedef TDateTime DateTime;


#endif