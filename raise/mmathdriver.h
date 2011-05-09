#ifndef MMATHDRIVER_H
#define MMATHDRIVER_H

#include <math.h>

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
};


#endif