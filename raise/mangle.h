#ifndef MANGLE_H
#define MANGLE_H

#include "raisetypes.h"
#include "mmathdriver.h"
#include "mvector2.h"

template <class T>
class MDegree;

/**
 * SI Angle measurement unit.
 */
template <class T>
class MRadian
{
public:
	T Radian;

	inline MRadian( float f )
	{
		Radian = f;
	}

	inline static float ToDegree(float rad)
	{
		return (rad * 180.0f) / PI;
	}

	inline T Sine() const
	{
		return MathDriver::Sine(Radian);
	}

	inline T Cosine() const
	{
		return MathDriver::Cosine(Radian);
	}

	inline T Tangent() const
	{
		return MathDriver::Tangent(Radian);
	}

	inline MVector2<T> GetVector (T radius) const
	{
		MVector2<T> result;
		result.X = radius * Cosine();
		result.Y = radius * Sine();
		return result;
	}
};

template <class T>
class MDegree: public MRadian<T>
{
public:
	T Degree;

	MDegree()
	{
		Degree = 0;
		this->Radian = 0;
	}

	MDegree( float f ): MRadian<T>(ToRadian(f))
	{
		Degree = f;
	}

	inline void SetDegree(float f)
	{
		Degree = f;
		this->Radian = ToRadian(f);
	}

	MDegree(const MRadian<T>& rad)
	{
		Degree = MRadian<T>::ToDegree(rad.Radian);
		this->Radian = rad.Radian;
	}

	inline static T ToRadian(T deg)
	{
		return (deg * PI) / 180.0f;
	}
};

typedef MDegree<float> DegreeAngle;

typedef MRadian<float> RadianAngle;

// SI UNIT
typedef RadianAngle Angle;

#endif