#ifndef TNUMBERFORMATINFO_H
#define TNUMBERFORMATINFO_H

#include "tstring.h"

class TNumberFormatInfo
{
public:
	int CurrencyDecimalDigits;
	String CurrencyDecimalSeparator;
	String CurrencyGroupSeparator;
	int CurrencyNegativePattern;
	int CurrencyPositivePattern;
	String CurrencySymbol;
	String NaNSymbol;
	ch32* NativeDigits;
	String NegativeInfinitySymbol;
	String NegativeSign;
	int NumberDecimalDigits;
	String NumberDecimalSeparator;
	String NumberGroupSeparator;
	int NumberNegativePattern;
	int PercentDecimalDigits;
	String PercentDecimalSeparator;
	String PercentGroupSeparator;
	int PercentNegativePattern;
	int PercentPositivePattern;
	String PercentSymbol;
	String PerMilleSymbol;
	String PositiveInfinitySymbol;
	String PositiveSign;
};

#endif