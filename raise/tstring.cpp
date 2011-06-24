#include "stdafx.h"
#include "tstring.h"

TString TString::Empty("");

bool TString::Have( ch32 character ) const
{
	byte* start = Data;
	ch32 curChr;
	do 
	{
		curChr = StringDriver::DecodeAdv(start);
		if (curChr == character)
		{
			return true;
		}
	} while (curChr > 0);

	return false;
}

bool TString::Have( const TString& any ) const
{
	byte* start = Data;
	byte* tstart;
	ch32 curChr;
	ch32 curTg;
	do
	{
		curChr = StringDriver::DecodeAdv(start);
		tstart = any.Data;
		do 
		{
			curTg = StringDriver::DecodeAdv(tstart);
			if (curChr == curTg)
			{
				return true;
			}
		} while (curTg > 0);

	} while(curChr > 0);
	return false;
}

TString TString::ToLower() const
{
	TString result (Capacity + (Capacity / 2)); // 1.5x cap
	byte* src = Data;
	byte* end = src + ByteLength;

	TChar curChar;

	while(src <= end)
	{
		curChar = StringDriver::DecodeAdv(src);
		curChar = curChar.ToLower();
		result += curChar;
	}

	return result;
}

TString TString::ToUpper() const
{
	TString result (Capacity + (Capacity / 2)); // 1.5x cap
	byte* src = Data;
	byte* end = src + ByteLength;

	TChar curChar;
	while(src <= end)
	{
		curChar = StringDriver::DecodeAdv(src);
		curChar = curChar.ToUpper();
		result += curChar;
	}

	return result;
}

TString TString::Substring( int startIndex, int lengt ) const
{
	if (startIndex + lengt > Length)
	{
		lengt = Length - startIndex;
	}

	if (lengt <= 0)
	{
		return Empty; // string
	}


	byte* cpyStart = Data;

	if (startIndex > 0)
	{
		StringDriver::Count(Data,startIndex);
	}

	byte* cpyEnd = StringDriver::Count(cpyStart,lengt);
	int copyLength = cpyEnd - cpyStart;

	TString result(copyLength);
	MemoryDriver::Copy(result.Data,cpyStart,copyLength);
	result.ByteLength = copyLength;
	result.Length = lengt;
	result.Data[lengt] = 0;
	return result;
}