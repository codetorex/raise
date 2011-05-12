#include "stdafx.h"
#include "tutf8encoding.h"

static const dword MaximumChar = 0x7FFFFFFF;

static const byte bytesFromUTF8[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 0, 0
};

void UTF8Encoding::Length( const byte* data, dword& length, dword& bytelength )
{
	length = 0;
	bytelength = 0;

	for (byte cur=data[0]; cur > 0; cur = *(++data) )
	{
		length++;
		bytelength += bytesFromUTF8[cur]
	}

	// OLD ALGORITHM
	/*dword ibefore = 0;

	while (data[bytelength] > 0)
	ascii:  bytelength++;

	length += bytelength-ibefore;
	while (data[bytelength])
	{
		if (data[bytelength] > 0)
		{
			ibefore = bytelength;
			goto ascii;
		}
		else switch (0xF0 & s[bytelength]) 
		{
		case 0xE0: bytelength += 3; break;
		case 0xF0: bytelength += 4; break;
		default:   bytelength += 2; break;
		}
		++length;
	}*/
}

int UTF8Encoding::GetByteCount( const ch32* data,int length )
{
	int result = 0;

	while(length--)
	{
		dword ch = data[length];

		if      (ch < 0x80        ) {result += 1;}
		else if (ch < 0x800       ) {result += 2;}
		else if (ch < 0x10000     ) {result += 3;}
		else if (ch < 0x200000    ) {result += 4;}
		else if (ch < 0x4000000   ) {result += 5;}
		else if (ch <= MaximumChar) {result += 6;}
		else 
		{
			result += 2; // replacement character
		}
	}
	return result;
}

