#ifndef TCULTUREINFO_H
#define TCULTUREINFO_H

#include "tdatetimeformatinfo.h"
#include "tnumberformatinfo.h"

class TCultureInfo
{
public:
	static TCultureInfo& CurrentCulture();
	static TCultureInfo& InvariantCulture();

	String Name;

	String ThreeLetterISOLanguageName;
	String TwoLetterISOLanguageName;

	TDateTimeFormatInfo DateTimeFormat;
	TNumberFormatInfo NumberFormat;
};

#endif