#ifndef MMATHDRIVER_H
#define MMATHDRIVER_H

//#include <cmath>
#include <stdlib.h>
#include <math.h>
#include "m64bit.h"

class MathDriver
{
public:

	/// Single PI
/*	static const float PI;// = 3.1415926535897f;

	inline static float PI()
	{
		return 3.1415926535897f;
	}

	const static float HalfPI;

	/// Double PI
	const static double dPI;*/



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

	template<class T>
	inline static T Sine(T value)
	{
		return sin(value);
	}

	template<class T>
	inline static T Cosine(T value)
	{
		return cos(value);
	}

	template<class T>
	inline static T Tangent(T value)
	{
		return tan(value);
	}

	template<class T>
	inline static T Absolute(T value)
	{
		return fabs(value);
	}

	template<class T>
	inline static T Cotangent(T value)
	{
		return ((T)1.0f / Tangent(value));
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

	inline static double SquareRoot(double value)
	{
		return sqrt(value);
	}

	inline static float SquareRoot(float value)
	{
		return sqrtf(value);
	}

	/**
	 * A simple random number generator, that generates between 0 and 32767.
	 */
	inline static ui32 Random()
	{
		return rand();
	}

	template<class T>
	inline static T Clamp(T minValue,T maxValue, T value)
	{
		return value < minValue ? minValue : (value > maxValue ? maxValue : value);
	}

	template<class T>
	inline static T Min(T value1,T value2)
	{
		return value1 < value2 ? value1 : value2;
	}

	template<class T>
	inline static T Max(T value1,T value2)
	{
		return value1 > value2 ? value1 : value2;
	}

	template<class T>
	inline static bool Compare(T value1, T value2, T epsilon)
	{
		float diff = value2 - value1;
		if ( diff < epsilon && diff > -epsilon)
		{
			return true;
		}
		return false;
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