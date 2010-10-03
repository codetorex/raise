#ifndef TBINARY_H
#define TBINARY_H

#include "raisetypes.h"

#define NOTNEEDED

class RDLL TBinary
{
public:
	dword value;

	TBinary(dword _value)
	{
		value = _value;
	}


	/**
	* Log base 2 of value by lookup table. Use this if you know value is 2^n
	*/
	inline int log2k()
	{
		static const int MultiplyDeBruijnBitPosition2[32] = 
		{
			0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
			31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
		};
		return (MultiplyDeBruijnBitPosition2[(dword)(value * 0x077CB531U) >> 27]);
	}


	/**
	* Log base 2 of value. By lookup table.
	*/
	inline int log2()
	{
		register dword v = value;
		static const int MultiplyDeBruijnBitPosition[32] = 
		{
			0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
			8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
		};

		v |= v >> 1; // first round down to one less than a power of 2 
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;

		return MultiplyDeBruijnBitPosition[(dword)(v * 0x07C4ACDDU) >> 27];
	}



	/**
	* Checks if value is 2^n
	*/
	inline bool is2n()
	{
		if (popcount() == 1)
		{
			return true;
		}
		return false;
	}

	/**
	* Optimized bit population count.
	*/
	inline int popcount()
	{
		register dword v = value;
		v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
		v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
		dword c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
		return c;
	}


#ifndef NOTNEEDED

	/**
	* Calculates log base 2 for integer value naive way.
	*/
	inline int log2_naive()
	{
		register dword vcopy = value;
		int r = 0;
		while(vcopy>>=1)
			r++;
		return r;
	}
	
	/**
	* Calculates log base 2 for integer value another way.
	*/
	inline int log2_calc()
	{
		dword v = value;
		register unsigned int r; // result of log2(v) will go here
		register unsigned int shift;

		r =     (v > 0xFFFF) << 4; v >>= r;
		shift = (v > 0xFF  ) << 3; v >>= shift; r |= shift;
		shift = (v > 0xF   ) << 2; v >>= shift; r |= shift;
		shift = (v > 0x3   ) << 1; v >>= shift; r |= shift;
		r |= (v >> 1);
		return r;
	}


	/**
	* Counts bit population with naive way.
	*/
	inline int popcount_naive()
	{
		int bits = 32;
		register dword v = value;
		int c = 0;
		while(bits--)
		{
			c += v & 1;
			v >>= 1;
		}
		return c;
	}

	/**
	* Counts bit population with Brian Kernighan's way.
	*/
	inline int popcount_brian()
	{
		register dword v = value;
		dword c; // c accumulates the total bits set in v
		for (c = 0; v; c++)
		{
			v &= v - 1; // clear the least significant bit set
		}
		return c;
	}


	/**
	* Counts bit population with lookup table.
	*/
	inline int popcount_lookup()
	{
		static const unsigned char BitsSetTable256[256] = 
		{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
			B6(0), B6(1), B6(1), B6(2)
		};

		register dword v = value;
		int c; // c is the total bits set in v

		unsigned char * p = (unsigned char *) &v;
		c = BitsSetTable256[p[0]] + 
			BitsSetTable256[p[1]] + 
			BitsSetTable256[p[2]] +	
			BitsSetTable256[p[3]];
		return c;
	}
#endif

};

#endif