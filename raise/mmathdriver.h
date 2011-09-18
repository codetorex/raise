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

	inline static float Clamp(float minValue,float maxValue, float value)
	{
		return value < minValue ? minValue : (value > maxValue ? maxValue : value);
	}

	template<class T>
	inline static int Min(T value1,T value2)
	{
		return value1 < value2 ? value1 : value2;
	}
};

class MathInterpolate
{
public:
	inline static float LinearInterpolate(float y1,float y2,float mu)
	{
		return(y1*(1-mu)+y2*mu);
	}

	inline static float CosineInterpolate(float y1,float y2,float mu)
	{
		float mu2 = (1-cos(mu*PI))/2;
		return(y1*(1-mu2)+y2*mu2);
	}

	inline static float CubicInterpolate(float y0,float y1,float y2,float y3,float mu)
	{
		float a0,a1,a2,a3,mu2;

		mu2 = mu*mu;
		a0 = y3 - y2 - y0 + y1;
		a1 = y0 - y1 - a0;
		a2 = y2 - y0;
		a3 = y1;

		return(a0*mu*mu2+a1*mu2+a2*mu+a3);
	}

	inline static float HermiteInterpolate(float y0, float y1, float y2, float y3, float mu, float tension, float bias)
	{
		float m0,m1,mu2,mu3;
		float a0,a1,a2,a3;

		mu2 = mu * mu;
		mu3 = mu2 * mu;
		m0  = (y1-y0)*(1+bias)*(1-tension)/2;
		m0 += (y2-y1)*(1-bias)*(1-tension)/2;
		m1  = (y2-y1)*(1+bias)*(1-tension)/2;
		m1 += (y3-y2)*(1-bias)*(1-tension)/2;
		a0 =  2*mu3 - 3*mu2 + 1;
		a1 =    mu3 - 2*mu2 + mu;
		a2 =    mu3 -   mu2;
		a3 = -2*mu3 + 3*mu2;

		return(a0*y1+a1*m0+a2*m1+a3*y2);
	}
};


#endif