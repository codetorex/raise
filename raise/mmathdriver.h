#ifndef MMATHDRIVER_H
#define MMATHDRIVER_H

//#include <cmath>
#include <stdlib.h>
#include <math.h>
#include "m64bit.h"

#define PI			3.1415926535897f
#define DOUBLEPI	6.2831853071794f
#define HALFPI		1.5707963267948f

class MathInterpolateImplementation
{
public:
	inline static float Linear(float y1,float y2,float mu)
	{
		return(y1*(1-mu)+y2*mu);
	}

	inline static float Cosine(float y1,float y2,float mu)
	{
		float mu2 = (1-cos(mu*3.1415926535897f))/2;
		return(y1*(1-mu2)+y2*mu2);
	}

	inline static float Cubic(float y0,float y1,float y2,float y3,float mu)
	{
		float a0,a1,a2,a3,mu2;

		mu2 = mu*mu;
		a0 = y3 - y2 - y0 + y1;
		a1 = y0 - y1 - a0;
		a2 = y2 - y0;
		a3 = y1;

		return(a0*mu*mu2+a1*mu2+a2*mu+a3);
	}

	inline static float Hermite(float y0, float y1, float y2, float y3, float mu, float tension, float bias)
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


class MathImplementation
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


	MathInterpolateImplementation Interpolate;


	template<class T>
	inline static T Ceil (T value)
	{
		return ceil(value);
	}
	
	inline static float Ceil (float value)
	{
		return ceilf(value);
	}
	
	template<class T>
	inline static T Floor (T value)
	{
		return floor(value);
	}
	
	inline static float Floor(float value)
	{
		return floorf(value);
	}

	/**
	 * Logarithm function
     * @param value value of logarithm to be calculated
     * @return natural logarithm of x
     */
	template<class T>
	inline static T Log(T value)
	{
		return log(value);
	}
	
	
	inline static float Log(float value)
	{
		return logf(value);
	}

	/**
	 * Exponential function
     * @param value value of the exponent
     * @return exponential of value.
     */
	template<class T>
	inline static T Exp(T value)
	{
		return exp(value);
	}
	
	inline static float Exp(float value)
	{
		return expf(value);
	}


	/**
	 * Sine function.
     * @param value angle in radian
     * @return sine of given angle
     */
	template<class T>
	inline static T Sin(T value)
	{
		return sin(value);
	}
	
	inline static float Sin(float value)
	{
		return sinf(value);
	}

	/**
	 * Cosine function.
     * @param value angle in radian
     * @return cosine of given angle
     */
	template<class T>
	inline static T Cos(T value)
	{
		return cos(value);
	}

	inline static float Cos(float value)
	{
		return cosf(value);
	}

	
	/**
	 * Tangent function
     * @param value angle in radian
     * @return tangent of given angle
     */
	template<class T>
	inline static T Tan(T value)
	{
		return tan(value);
	}
	
	inline static float Tan(float value)
	{
		return tanf(value);
	}

	/**
	 * Absolute function
     * @param value
     * @return absolute of given value
     */
	template<class T>
	inline static T Abs(T value)
	{
		return fabs(value);
	}
	
	inline static float Abs(float value)
	{
		return fabsf(value);
	}

	/**
	 * Cotangent function
     * @param value angle in radian
     * @return Cotangent of value
     */
	template<class T>
	inline static T Cot(T value)
	{
		return ((T)1.0f / Tan(value));
	}

	template<class T>
	inline static T Round(T value)
	{
		return (value > 0.0) ? Floor(value + 0.5) : Ceil(value - 0.5);
	}
	
	inline static int64 RoundToInt64(double value)
	{
		if (value >= 0) return (int64) (value+0.5);
		return (int64) (value-0.5);
	}

	/**
	 * Square root function
     * @param value value of square root to be calculated
     * @return square root of value
     */
	template<class T>
	inline static T Sqrt(T value)
	{
		return sqrt(value);
	}

	inline static float Sqrt(float value)
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

	/**
	 * Returns min of two values
     */
	template<class T>
	inline static T Min(T value1,T value2)
	{
		return value1 < value2 ? value1 : value2;
	}
	
	/**
	 * Returns min of three values
     */
	template<class T>
	inline static T Min(T value1,T value2, T value3)
	{
		return Min( value1, Min(value2,value3) );
	}
	
	/**
	 * Return max of two values
     */
	template<class T>
	inline static T Max(T value1,T value2)
	{
		return value1 > value2 ? value1 : value2;
	}
	
	/**
	 * Return max of three values
     */
	template<class T>
	inline static T Max(T value1,T value2, T value3)
	{
		return Max( value1, Max(value2,value3) );
	}

	/**
	 * Compares two floating point values with given epsilon
     * @return true if difference of two values is under epsilon
     */
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



extern MathImplementation Math;





#endif