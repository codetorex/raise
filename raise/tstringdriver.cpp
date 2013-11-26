#include "stdafx.h"
#include "tstringdriver.h"

static const ch32 s_Whitespaces[] = { 0x20, 0x09, 0x0A, 0x0D, 0x0B, 0x0 };
const Array<ch32> StringDriver::Whitespaces(s_Whitespaces);

static const ui32 MaximumChar = 0x7FFFFFFF;

int StringDriver::RequiredBytes( const ch32* data,int length )
{
	int result = 0;

	while(length--)
	{
		ui32 ch = data[length];

		if      (ch < 0x80        ) {result += 1;}
		else if (ch < 0x800       ) {result += 2;}
		else if (ch < 0x10000     ) {result += 3;}
		else if (ch < 0x200000    ) {result += 4;}
		/*else if (ch < 0x4000000   ) {result += 5;}
		else if (ch <= MaximumChar) {result += 6;} UTF8 is max 4 bytes*/
		else 
		{
			result += 2; // replacement character
		}
	}
	return result;
}

int StringDriver::EncodeOnlyUnicode( byte* charptr, ch32 c )
{
	if (c <= 0x7FF)
	{
		charptr[0] = (byte)(c >> 6 & 0x1F | 0xC0 );
		charptr[1] = (byte)(c      & 0x3F | 0x80 );
		return 2;
	}
	else if (c <= 0xFFFF)
	{
		charptr[0] = (byte)(c >> 12 & 0x0F | 0xE0 );
		charptr[1] = (byte)(c >> 6  & 0x3F | 0x80 );
		charptr[2] = (byte)(c       & 0x3F | 0x80 );
		return 3;
	}
	else if (c <=  0x10FFFF)
	{
		charptr[0] = (byte)(c >> 18 & 0x07 | 0xF0 );
		charptr[1] = (byte)(c >> 12 & 0x3F | 0x80 );
		charptr[2] = (byte)(c >> 6  & 0x3F | 0x80 );
		charptr[3] = (byte)(c       & 0x3F | 0x80 );
		return 4;
	}

	// Invalid character, so replace it with replacement character...
	charptr[0] = 0xE6;
	charptr[1] = 0x96;
	charptr[2] = 0x87;
	return 3;
}

ch32 StringDriver::DecodeOnlyUnicode( const byte* charptr, int& byteLength )
{
	byteLength = bytesFromUTF8[ charptr[0] ];
	ch32 result = *charptr & ((1<<(7-byteLength))-1); // leading byte
	switch(byteLength)
	{
	case 4:
		result = (result<<6) | (*(++charptr) & 0x3F);
	case 3:
		result = (result<<6) | (*(++charptr) & 0x3F);
	case 2:
		result = (result<<6) | (*(++charptr) & 0x3F);
		break;
	}
	return result;
}