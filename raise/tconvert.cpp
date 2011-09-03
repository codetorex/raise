#include "stdafx.h"
#include "tconvert.h"
#include "texception.h"


int TConvert::ToCharArray( int value, char* dst , int dstCap )
{
	int division = value;

	char tmp[16];
	char* out = tmp;
	do 
	{
		int rem = division % 10;
		division /= 10;
		*(out++) = rem + '0';
	} while (division);

	if (value < 0)
	{
		*out++ = '-';
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

int TConvert:: ToInt32Ambiguous( const TString& value, int startIndex,int* numLength, bool skipBegin )
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