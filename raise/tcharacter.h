#ifndef TCHARACTER_H
#define TCHARACTER_H

#include "raisetypes.h"

typedef char	ch8;
typedef wchar_t ch16;
typedef ui32	ch32;


class TChar
{
public:
	ch32 Character;

	TChar()
	{
		Character = MXDWORD; // means un initialized.
	}

	TChar(ch32 value)
	{ 
		Character = value;
	}

	inline ch32 ToLower()
	{
		if('A' <= Character && Character <= 'Z') 
		{
			return (Character + ('a'-'A'));
		}
		else if (Character < 0x80)
		{
			return Character;
		}
		return '?';
	}

	inline ch32 ToUpper()
	{
		if('a' <= Character && Character <= 'z') 
		{
			return (Character - ('a'-'A'));
		}
		else if (Character < 0x80)
		{
			return Character;
		}
		return '?';
	}

	inline bool Compare(ch32 c)
	{
		return Character == c;
	}

	inline TChar& operator = (ch32 c)
	{
		Character = c;
		return *this;
	}

	// Unicode related code should be here ( ToLower, ToUpper, Compare?, Collate?)
};

#endif