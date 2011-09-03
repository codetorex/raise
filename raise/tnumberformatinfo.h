#ifndef TNUMBERFORMATINFO_H
#define TNUMBERFORMATINFO_H

#include "tstring.h"

class TNumberFormatInfo
{
public:
	int CurrencyDecimalDigits;
	TString CurrencyDecimalSeparator;
	TString CurrencyGroupSeparator;
	int CurrencyNegativePattern;
	int CurrencyPositivePattern;
	TString CurrencySymbol;
	TString NaNSymbol;
	ch32* NativeDigits;
	TString NegativeInfinitySymbol;
	TString NegativeSign;
	int NumberDecimalDigits;
	TString NumberDecimalSeparator;
	TString NumberGroupSeparator;
	int NumberNegativePattern;
	int PercentDecimalDigits;
	TString PercentDecimalSeparator;
	TString PercentGroupSeparator;
	int PercentNegativePattern;
	int PercentPositivePattern;
	TString PercentSymbol;
	TString PerMilleSymbol;
	TString PositiveInfinitySymbol;
	TString PositiveSign;
};

#endif