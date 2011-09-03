#ifndef TCULTUREINFO_H
#define TCULTUREINFO_H

#include "tdatetimeformatinfo.h"
#include "tnumberformatinfo.h"

class TCultureInfo
{
public:
	static TCultureInfo& CurrentCulture();
	static TCultureInfo& InvariantCulture();

	TString Name;

	TString ThreeLetterISOLanguageName;
	TString TwoLetterISOLanguageName;

	TDateTimeFormatInfo DateTimeFormat;
	TNumberFormatInfo NumberFormat;
};

#endif