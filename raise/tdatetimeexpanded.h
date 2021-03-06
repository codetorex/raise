#ifndef TDATETIMEEXPANDED_H
#define TDATETIMEEXPANDED_H

#include "raisetypes.h"
#include "tstring.h"
#include "tconvert.h"

class TDateTime;
class TDateTimeUnix;

class TDateTimeExpanded
{
public:
	static const String DayNames[]; // TODO: make this culture specific

	int MicroSecond;
	int MilliSecond; // 10 bits
	int Second; // 6 bits
	int Minute; // 6 bits
	int Hour; // 5 bits
	int Day; // 5 bits
	int Month; // 4 bits
	int DayOfYear; // 9 bits
	int DayOfWeek; // 3 bits
	int Year; // remaining bits
	bool Leap; // 1 bit
	bool LocalTime;

	/**
	 * Converts TDateTime to expanded form.
	 * Makes LocalTime to false since TDateTime is UTC only.
	 */
	TDateTimeExpanded& operator = (const TDateTime& value);
	TDateTimeExpanded& operator = (const TDateTimeUnix& value);
	TDateTimeExpanded& operator = (const TDateTimeWindows& value);

	void Set(const TDateTime& time, bool convertToLocal = false);

	TDateTimeExpanded(const TDateTime& time, bool convertToLocal = false)
	{
		Set(time,convertToLocal);
	}

	/*inline static TString GetDayName(int dayofweek)
	{
		return DayNames[dayofweek];
	}*/

	void ToLocalTime();

	inline String GetDayName() const
	{
		return DayNames[DayOfWeek];
	}

	String ToString() const
	{
		TStringBuilderStack<20> str;

		str.AppendPadded(Month,2);
		str.AppendChar('/');
		str.AppendPadded(Day,2);
		str.AppendChar('/');
		str.AppendPadded(Year,4);
		str.AppendChar(' ');
		str.AppendPadded(Hour,2);
		str.AppendChar(':');
		str.AppendPadded(Minute,2);
		str.AppendChar(':');
		str.AppendPadded(Second,2);

		String strresult = str.ToString();

		return strresult;
	}

	String ToStringFileName() const
	{
		TStringBuilderStack<20> str;
		
		str.AppendPadded(Year,4);
		str.AppendPadded(Month,2);
		str.AppendPadded(Day,2);
		str.AppendPadded(Hour,2);
		str.AppendPadded(Minute,2);
		str.AppendPadded(Second,2);
		
		String strresult = str.ToString();
		return strresult;
	}

	String ToStringUniversal() const
	{
		TStringBuilderStack<20> str;
		str.AppendPadded(Year,4);
		str.AppendChar('-');
		str.AppendPadded(Month,2);
		str.AppendChar('-');
		str.AppendPadded(Day,2);
		str.AppendChar(' ');
		str.AppendPadded(Hour,2);
		str.AppendChar(':');
		str.AppendPadded(Minute,2);
		str.AppendChar(':');
		str.AppendPadded(Second,2);

		String strresult = str.ToString();
		return strresult;
	}

	String ToString(const String& format) const
	{
		if (format.Length == 1)
		{
			int stdfmt = format.Data[0];
			switch(stdfmt)
			{
			case 'u':
				return ToStringUniversal();
				break;
			}
		}


	}
};

#endif