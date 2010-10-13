#ifndef TBITFIELD_H
#define TBITFIELD_H

#include "tbinary.h"

/**
* 32bit bit field manipulation.
*/
class TBitfield
{
public:
	dword* bin;
	int fieldcount;
	int* fieldstart;
	int* fieldlength;

	TBitfield(int fields)
	{
		fieldstart = new int [fields];
		fieldlength = new int [fields];
		fieldcount = 0;
	}

	TBitfield(int fcount,int* starts,int* lengths)
	{
		fieldcount = fcount;
		fieldstart = starts;
		fieldlength = lengths;
	}

	inline int addfield(int start,int length)
	{
		fieldstart[fieldcount] = start;
		fieldlength[fieldcount] = length;
		fieldcount++;
	}

	inline int getvalue(int which)
	{
		register dword v = *bin;
		v >>= fieldstart[which];
		v &= bitmasks[ fieldlength[which] ];
		return v;
	}

	inline void setvalue(int which,int value)
	{
		// set those bits 0 first
		dword bitmask = bitmasks[ fieldlength[which] ];
		dword start = fieldstart[which];
		*bin &= ~(bitmask << start);

		// set new value
		value &= bitmask;
		value <<= start;
		*bin |= value;
	}
};

typedef TBitfield bitfd; // bit field short hand type definition

#endif

