#include "stdafx.h"
#include "tstring.h"
#include "tconvert.h"

int TConvert::ToCharArrayInt( int value, char* dst , int dstCap )
{
	int division = value;

	bool negative = division < 0;
	if (negative) division = -division;

	char tmp[16];
	char* out = tmp;
	do 
	{
		int rem = division % 10;
		division /= 10;
		*(out++) = rem + '0';
	} while (division);

	if (negative)
	{
		*(out++) = '-';
	}

	*out = 0;

	if (dstCap < (out - tmp) )
	{
		throw Exception("Capacity not enough");
	}

	char* dstw = dst;
	while( tmp < out )
	{
		*(dstw++) = *(--out);
	}
	return (dstw - dst);
}

int TConvert::ToInt32Split( const TString& value, ch32 splitChar, TArray<int>& results )
{
	if (!value.IsASCII())
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	int matchCount = 0;

	char numbers[32];
	int numLen = 0;

	for (int i=0;i<value.Length;i++)
	{
		char curChar = value.Data[i];
		if (curChar == splitChar)
		{
			matchCount++;
			if (numLen > 0)
			{
				int curResult = ToInt32(numbers);
				results.Add(curResult);
				numLen = 0;
			}
			continue;
		}

		// skip non-numbers?
		if (curChar != '-' && !(curChar >= '0' && curChar <= '9'))
		{
			continue;
		}

		numbers[numLen++] = curChar;
	}

	if (numLen > 0)
	{
		int curResult = ToInt32(numbers);
		results.Add(curResult);
	}

	return matchCount;
}

int TConvert::ToInt32Ambiguous( const TString& value, int startIndex,int* numLength, bool skipBegin )
{
	TCharacterEnumerator schars(value);

	if (startIndex > 0)
	{
		schars.SkipChars(startIndex);
	}


	char tmp[16];
	int numl = 0;

	if (skipBegin)
	{
		while(schars.MoveNext())
		{
			if (schars.Current >= '0' && schars.Current <= '9')
			{
				tmp[numl++] = schars.Current;
				break;
			}
		}
	}

	while(schars.MoveNext())
	{
		if (schars.Current >= '0' && schars.Current <= '9')
		{
			tmp[numl++] = schars.Current;
		}
		else
		{
			break;
		}
	}
	tmp[numl] = 0;

	int result = ToInt32(tmp);
	if(numLength != 0 )
	{
		*numLength = numl;
	}
	return result;
}

int TConvert::ToInt32( char* value )
{
	return atoi(value);
}

int TConvert::ToInt32( const TString& value )
{
	if(value.IsASCII() )
	{
		return ToInt32( (char*)value.Data );
	}
	throw Exception("Problem in int");
}

ui32 TConvert::ToUInt32Hex( const TString& value )
{
	if (!value.IsASCII())
	{
		throw Exception("Problem in hex");
	}

	TStringBuilderStack<32> sb;
	sb.Append(value);
	const byte* sbData = sb.GetData();

	int rem = value.Length;
	if (sbData[1] == 'x' || sbData[1] == 'X')
	{
		sbData += 2;
		rem -= 2;
	}

	ui32 result = 0;
	for (int i=0;i<rem;i++)
	{
		result <<= 4; // it gives us 4 bits
		ui32 val = DecodeHexChar(sbData[i]);
		result |= val;
	}

	return result;
}

ui32 TConvert::DecodeHexChar( char val )
{
	if (val >= '0' && val <= '9')
	{
		return val - '0';
	}
	else if (val >= 'a' && val <= 'f')
	{
		return (val - 'a') + 10;
	}
	else if (val >= 'A' && val <= 'F')
	{
		return (val - 'A') + 10;
	}
	throw Exception("Wrong hex value");
}

