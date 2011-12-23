#ifndef TDATETIMEEXPANDED_H
#define TDATETIMEEXPANDED_H

#include "raisetypes.h"
#include "tstringbuilder.h"
#include "tconvert.h"

class TDateTime;
class TDateTimeUnix;

class TDateTimeExpanded
{
public:
	static const TString DayNames[]; // TODO: make this culture specific

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

	inline TString GetDayName() const
	{
		return DayNames[DayOfWeek];
	}

	TString ToString() const
	{
		byte result[20];

		TStringBuilder str(result,20);
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

		TString strresult = str.ToString();
		str.UnbindByteArray();

		return strresult;
	}

	TString ToStringFileName() const
	{
		byte result[20];
		TStringBuilder str(result,20);
		str.AppendPadded(Year,4);
		str.AppendPadded(Month,2);
		str.AppendPadded(Day,2);
		str.AppendPadded(Hour,2);
		str.AppendPadded(Minute,2);
		str.AppendPadded(Second,2);
		
		TString strresult = str.ToString();
		str.UnbindByteArray();
		return strresult;
	}

	TString ToStringUniversal() const
	{
		byte result[20];

		TStringBuilder str(result,20);
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

		TString strresult = str.ToString();
		str.UnbindByteArray();

		return strresult;
	}

	TString ToString(string format) const
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