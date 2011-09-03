#include "stdafx.h"
#include "tdatetime.h"
#include "texception.h"
#include "ttimedriver.h"


const TString TDateTimeExpanded::DayNames[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
const int TDateTime::MonthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
const int TDateTime::MonthStartDay[] = {0,31,59,90,120,151,181,212,243,273,304,334};
const int TDateTime::MonthStartLeap[] = {0,31,60,91,121,152,182,213,244,274,305,335};


TDateTimeExpanded& TDateTimeExpanded::operator=( const TDateTime& value )
{
	int64 microdata = (value.Ticks % 86400000000LL);
	dword datedata = (dword)(value.Ticks / 86400000000LL);

	MicroSecond = microdata % 1000;
	microdata /= 1000;
	dword timedata = (dword)microdata;

	MilliSecond = timedata % 1000;
	timedata /= 1000;
	Second = timedata % 60;
	timedata /= 60;
	Minute = timedata % 60;
	timedata /= 60;
	Hour = timedata;

	int leapdays =  TDateTime::GetLeapDays(datedata);
	dword sdays = datedata - leapdays;
	dword years = sdays / 365;
	dword rdays = sdays % 365;

	Year = years + 1;
	Leap = false;
	if (DateTime::IsLeapYear(Year))
	{
		Leap = true;
	}

	Day = rdays;

	if (Day >= 31)
	{
		Day -= 31;

		int febdays = Leap ? 29: 28;
		if (Day >= febdays)
		{
			Day -= febdays;
			if (Day >= 31)
			{
				Day -= 31;
				if (Day >= 30)
				{
					Day -= 30;
					if (Day >= 31)
					{
						Day -= 31;
						if (Day >= 30)
						{
							Day -= 30;
							if (Day >= 31)
							{
								Day -= 31;
								if (Day >= 31)
								{
									Day -= 31;
									if (Day >= 30)
									{
										Day -= 30;
										if (Day >= 31)
										{
											Day -= 31;
											if (Day >= 30)
											{
												Day -= 30;
												Month = 12;
											}
											else
											{
												Month = 11;
											}
										}
										else
										{
											Month = 10;
										}
									}
									else
									{
										Month = 9;
									}
								}
								else
								{
									Month = 8;
								}
							}
							else
							{
								Month = 7;
							}
						}
						else
						{
							Month = 6;
						}
					}
					else
					{
						Month = 5;
					}
				}
				else
				{
					Month = 4;
				}
			}
			else
			{
				Month = 3;
			}
		}
		else
		{
			Month = 2;
		}
	}
	else
	{
		Month = 1;
	}

	Day++;
	DayOfWeek = datedata % 7;
	DayOfYear = rdays + 1;

	LocalTime = false;

	return *this;
}

TDateTimeExpanded& TDateTimeExpanded::operator=( const TDateTimeUnix& value )
{
	TDateTime dt = value;
	*this = dt;
	LocalTime = false;
	return *this;
}

TDateTimeExpanded& TDateTimeExpanded::operator=(const TDateTimeWindows& value)
{
	TDateTime dt = value;
	*this = dt;
	LocalTime = false;
	return *this;
}

TDateTimeUnix& TDateTimeUnix::operator=( const TDateTime& value )
{
	int64 tmp = value.Ticks / 1000000;
	tmp -= 719162LL * 24 * 60 * 60; // 719162 * 24 * 60 * 60 * 1000000;
	TimeStamp = (dword)tmp;
	return *this;
}

void TDateTime::AddTime( int hours,int minutes, int seconds,int milliseconds,int microseconds )
{
	int64 timedata = (int64)((hours * 60 * 60) + (minutes * 60) + (seconds));
	timedata *= 1000000;
	timedata += ((int64)milliseconds * 1000) + microseconds;
	Ticks += timedata;
}

void TDateTime::SetDate( int year,int month,int day )
{
	int ldays = GetLeapYears(year) + ((year-1) * 365);
	ldays += day-1;
	month--;
	if (IsLeapYear(year))
	{
		ldays += MonthStartLeap[month];
		ldays--; // we added this day in first line because of getleapyears function.
	}
	else
	{
		ldays += MonthStartDay[month];
	}
	Ticks = (int64)(ldays) * 24 * 60 * 60;
	Ticks *= 1000000;
}

TDateTime& TDateTime::operator=( const TDateTimeUnix& value )
{
	Ticks = value.TimeStamp;
	Ticks += 719162LL * 24 * 60 * 60;
	Ticks *= 1000000;
	return *this;
}

TDateTime& TDateTime::AddDaysReal( double days )
{
	days *= 86400000000.0L;
	int64 dayticks = MathDriver::RoundToInt64(days);
	Ticks += dayticks;
	return *this;
}

TDateTimeUnix& TDateTimeUnix::operator = (const TDateTimeExpanded& value)
{
	TDateTime tmp = value;
	*this = tmp;
	return *this;
}

void TDateTimeExpanded::ToLocalTime()
{
	TDateTime ltime = TimeDriver::ToLocalTime(*this);
	*this = ltime;
	LocalTime = true;
}

void TDateTimeExpanded::Set(const TDateTime& time, bool convertToLocal /* = false */)
{
	if (convertToLocal)
	{
		TDateTime tmp = TimeDriver::ToLocalTime(time);
		*this = tmp;
		LocalTime = true;
	}
	else
	{
		*this = time;
		LocalTime = false;
	}
}