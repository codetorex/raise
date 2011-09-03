#ifndef M64BIT_H
#define M64BIT_H

#ifdef SYS64BIT

#ifdef WIN32
typedef __i64 qword;
#else
typedef long long qword;
#endif


#else

// I heard it was supported in regual c++ too after c99...
typedef unsigned long long qword;

typedef qword	uint64;
typedef signed long long int64;

//TODO: test qword class.

/**
* Provides 64bit mathematics for 32bit systems.
*/
/*class qword
{
public:
	dword low;
	int high;

	qword operator - (const qword other) const
	{
		qword diff;

		diff.high = high - other.high;
		diff.low = low - other.low;

		if (diff.low > low)
			--diff.high;
	
		return diff;
	}

	qword operator + (const qword other) const
	{
		qword sum;

		sum.high = high + other.high;
		sum.low = low + other.low;

		if (sum.low < low)
			++sum.high;
		
		return sum;
	}

	TString& ToString()
	{

	}
};*/

#endif

#endif
