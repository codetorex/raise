#ifndef TBITSTACK_H
#define TBITSTACK_H

#include "tbinary.h"

#ifdef LINUX
#include <stdlib.h>
#endif

/**
* 32bit bit stack implementation.
*/
class TBitstack
{
public:
	dword bin;

	TBitstack(dword _bin)
	{
		bin = _bin;
	}

	/**
	* Signed value push to bit stack.
	*/
	void pushs(int value,int bit)
	{
		dword data = abs(value);
		if ( data > bitmasks[bit-2] )
			throw "Value out of bounds.";
		
		if (value < 0)
		{
			data |= bits[bit-1]; // set sign bit
		}
		bin <<= bit;
		bin |= data;
	}

	/**
	* Signed value pop from bit stack.
	*/
	int pops(int bit)
	{
		int value = bin & bitmasks[bit-2];
		int sign = bin & bits[bit-1];
		bin >>= bit;

		if (sign) // value is negative
		{
			return -value;
		}
		return value;
	}

	/**
	* Unsigned value push to bit stack.
	*/
	void push(dword value,int bit)
	{
		if (value > bitmasks[bit-1])
			throw "Value out of bounds.";
		
		bin <<= bit;
		bin |= value;
	}

	/**
	* Unsigned value pop from bit stack.
	*/
	dword pop(int bit)
	{
		dword value = bin & bitmasks[bit-1];
		bin >>= bit;
		return value;
	}

	inline TString ToString()
	{
		return TBinary::ToString(bin);
	}
};

typedef TBitstack bitst;

#endif