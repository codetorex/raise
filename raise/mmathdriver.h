#ifndef MMATHDRIVER_H
#define MMATHDRIVER_H

#include <math.h>
#include "m64bit.h"

class MathDriver
{
public:

	inline static float Ceil(float value)
	{
		return ceil(value);
	}

	inline static float Log(float value)
	{
		return log(value);
	}

	inline static float Exp(float value)
	{
		return exp(value);
	}

	inline static double Round(double value)
	{
		return (value > 0.0) ? floor(value + 0.5) : ceil(value - 0.5);
	}

	inline static int64 RoundToInt64(double value)
	{
		if (value >= 0) return (int64) (value+0.5);
		return (int64) (value-0.5);
	}

	inline static int Min(int value1,int value2)
	{
		return value1 < value2 ? value1 : value2;
	}

	inline static int Min(dword value1,dword value2)
	{
		return value1 < value2 ? value1 : value2;
	}
};



#endif