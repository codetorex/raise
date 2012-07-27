#ifndef TBINARY_H
#define TBINARY_H

#include "raisetypes.h"
#include "tstring.h"

#define NOTNEEDED


enum BitEnum
{
	BIT00 = 0x1,
	BIT01 = 0x2,
	BIT02 = 0x4,
	BIT03 = 0x8,
	BIT04 = 0x10,
	BIT05 = 0x20,
	BIT06 = 0x40,
	BIT07 = 0x80,
	BIT08 = 0x100,
	BIT09 = 0x200,
	BIT10 = 0x400,
	BIT11 = 0x800,
	BIT12 = 0x1000,
	BIT13 = 0x2000,
	BIT14 = 0x4000,
	BIT15 = 0x8000,
	BIT16 = 0x10000,
	BIT17 = 0x20000,
	BIT18 = 0x40000,
	BIT19 = 0x80000,
	BIT20 = 0x100000,
	BIT21 = 0x200000,
	BIT22 = 0x400000,
	BIT23 = 0x800000,
	BIT24 = 0x1000000,
	BIT25 = 0x2000000,
	BIT26 = 0x4000000,
	BIT27 = 0x8000000,
	BIT28 = 0x10000000,
	BIT29 = 0x20000000,
	BIT30 = 0x40000000,
	BIT31 = 0x80000000
};

static const ui32 bits[] = 
{ 
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

static const ui32 bitmasks[] =
{
	0x00000001, 0x00000003, 0x00000007, 0x0000000f,
	0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff,
	0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff,
	0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff,
	0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff,
	0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
	0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff,
	0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff,
};

static const ui32 invbitmasks[] = 
{
	0xfffffffe, 0xfffffffc, 0xfffffff8, 0xfffffff0,
	0xffffffe0, 0xffffffc0, 0xffffff80, 0xffffff00,
	0xfffffe00, 0xfffffc00, 0xfffff800, 0xfffff000,
	0xffffe000, 0xffffc000, 0xffff8000, 0xffff0000,
	0xfffe0000, 0xfffc0000, 0xfff80000, 0xfff00000,
	0xffe00000, 0xffc00000, 0xff800000, 0xff000000,
	0xfe000000, 0xfc000000, 0xf8000000, 0xf0000000,
	0xe0000000, 0xc0000000, 0x80000000, 0x00000000,
};

/**
* 32bit binary value manipulation tools.
*/
class RDLL TBinary
{
public:
	ui32 value;

	TBinary()
	{
		value = 0;
	}

	TBinary(ui32 _value)
	{
		value = _value;
	}


	/**
	* Log base 2 of value by lookup table. Use this if you know value is 2^n
	*/
	inline int Log2Known()
	{
		static const int MultiplyDeBruijnBitPosition2[32] = 
		{
			0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
			31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
		};
		return (MultiplyDeBruijnBitPosition2[(ui32)(value * 0x077CB531U) >> 27]);
	}


	/**
	* Log base 2 of value. By lookup table.
	*/
	inline int Log2()
	{
		register ui32 v = value;
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

		return MultiplyDeBruijnBitPosition[(ui32)(v * 0x07C4ACDDU) >> 27];
	}



	/**
	* Returns if value is 2^n
	*/
	inline bool IsPowerOf2()
	{
		if (PopulationCount() == 1)
		{
			return true;
		}
		return false;
	}

	/**
	* Returns number of set bits
	*/
	inline int PopulationCount()
	{
		register ui32 v = value;
		v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
		v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
		ui32 c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
		return c;
	}

	/**
	* Returns string representation of binary.
	*/
	inline TString ToString()
	{
		return ToString(value);
	}

	static inline TString ToString(ui32 binvalue)
	{
		register ui32 v = binvalue;
		TString result(32);
		for (int i=0;i<32;i++)
		{
			result.AppendASCIIFast('0' + (v & 1));
			v >>= 1;
		}
		return result;
	}

	/**
	* Set value.
	*/
	/*inline TBinary& operator =  (dword _value)
	{
		value = _value;
		return *this;
	}*/


#ifndef NOTNEEDED

	/**
	* Calculates log base 2 for integer value naive way.
	*/
	inline int log2_naive()
	{
		register ui32 vcopy = value;
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
		ui32 v = value;
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
		register ui32 v = value;
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
		register ui32 v = value;
		ui32 c; // c accumulates the total bits set in v
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

		register ui32 v = value;
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

typedef TBinary bin32; // binary dword short hand definition

#endif