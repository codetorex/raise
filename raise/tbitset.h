#ifndef TBITARRAY_H
#define TBITARRAY_H

#include "raisetypes.h"

class TMemoryPool;

class TBitset
{
protected:
	dword*	Data;

public:
	int		DwordLength;
	int		Length;
	
	TBitset(dword* data,int length);
	TBitset(int length);

	int		SearchEmptyBits(int bitcount, int start);
	int		FillBits(int start,int length, bool value);
	char*	GetBitsAsChar(int start,int length);
	void	ClearBits();


	static dword CalculateDwordLengthFromBitLength(int bitlength);

private:
	void UseBitLength(int bitlength);
};



#endif