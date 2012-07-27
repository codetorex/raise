#include "stdafx.h"
#include "tbitset.h"
#include <list>

TBitset::TBitset( ui32* data,int length )
{
	UseBitLength(length);
	this->Data = data;
	ClearBits();
}

TBitset::TBitset( int length )
{
	UseBitLength(length);
	Data = new ui32[DwordLength];
	ClearBits();
}

void TBitset::ClearBits()
{
	for (int i=0;i<DwordLength;i++)
	{
		Data[i] = 0;
	}
}


ui32 TBitset::CalculateDwordLengthFromBitLength( int bitlength )
{
	int irrpart = bitlength % 32;
	int dwdpart = bitlength / 32;
	if (irrpart > 0)
		dwdpart++;

	return dwdpart;
}

void TBitset::UseBitLength( int bitlength )
{
	Length = bitlength;
	DwordLength = TBitset::CalculateDwordLengthFromBitLength(bitlength);
}

char* TBitset::GetBitsAsChar( int start,int length )
{
	int irrstart = MOD32(start);
	int relstart = DIV32( (start-irrstart) );
	ui32 k = 1 << irrstart;
	int j = irrstart;

	char* charBuf = new char[length+1];
	char* curChar = charBuf;
	charBuf[length] = 0;

	for (int i=relstart;i<DwordLength;i++)
	{
		ui32 dtemp = Data[i];
		for (;j<32;j++)
		{
			if ((dtemp & k) == 0)
			{
				*(curChar++) = '0';
			}
			else
			{
				*(curChar++) = '1';
			}
			k <<=1;

			if (--length == 0)
				return charBuf;
		}
		j = 0;
		k = 1;
	}

	return charBuf;
}

int TBitset::FillBits( int start,int length, bool value )
{
	int f=0;
	int irrstart = MOD32(start);
	int relstart = DIV32( (start-irrstart) );
	ui32 k = 1 << irrstart;
	int j = irrstart;

	for (int i=relstart;i<DwordLength;i++)
	{
		ui32 dtemp = Data[i];
		for (;j<32;j++)
		{
			if (value)
			{
				dtemp |= k;
			}
			else
			{
				dtemp &= ~k;
			}
			k <<= 1;

			if (--length == 0)
			{
				Data[i] = dtemp;
				return 0;
			}
		}
		Data[i] = dtemp;
		j = 0;
		k = 1;
	}
	return 1;// WTF... only happens from filling x to end of buffer
}

int TBitset::SearchEmptyBits( int bitcount, int start )
{
	int f = 0;
	int irrstart = MOD32(start);
	int relstart = DIV32( (start-irrstart) );
	ui32 k = 1 << irrstart;
	int j = irrstart;

	for (int i=relstart;i<DwordLength;i++)
	{
		ui32 dtemp = Data[i];
		if (dtemp == MXDWORD) // full
		{
			f = 0;
			continue;
		}
		for (;j<32;j++)
		{
			if ( (dtemp & k) == 0)
			{
				f++;
				if (f==bitcount)
				{
					return ((MUL32(i) + j + 1) - f);
				}
			}
			else
			{
				f = 0;
			}
			k <<= 1;
		}
		j = 0;
		k = 1;
	}

	return -1;
}