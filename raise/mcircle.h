#ifndef MCIRCLE_H
#define MCIRCLE_H

#include "raisetypes.h"
#include "mmathdriver.h"
#include "mline.h"
#include "mangle.h"

/**
 * Circle on a plane
 */
template <class T>
class MCircle2: public MVector2<T>
{
public:
	//MVector2<T> Position;
	T Radius;

	MCircle2()
	{
		set(0,0);
		Radius = 0;
	}

	MCircle2(T x, T y, T rad)
	{
		set(x,y);
		Radius = rad;
	}

	inline void SetDiameter(T dia)
	{
		Radius = dia / 2.0f;
	}

	inline T GetDiameter()
	{
		return Radius * 2.0f;
	}

	inline T GetArea()
	{
		return PI * Radius * Radius;
	}

	inline T GetPerimeter()
	{
		return 2.0f * PI * Radius;
	}

	inline MVector2<T> PointOnCircle( const MRadian<T>& rangle )
	{
		if (rangle.Radian == 0)
		{
			return MVector2<T>(x + Radius, y);
		}

		MVector2<T> v ( rangle.GetVector(this->Radius) );


		return ( v + (*this));
	}

	MLine2<T> RadiusLine( const MRadian<T>& rangle )
	{
		MLine2<T> ln; 
		ln.StartPoint = *this;
		ln.EndPoint = PointOnCircle(rangle);
		return ln;
	}

	inline T CircleDistance( const MCircle2<T>& other)
	{
		T totalDistance = Distance(other);
		return totalDistance - (Radius + other.Radius);
	}

	inline bool CollisionCheck( const MCircle2<T>& other)
	{
		if (CircleDistance(other) < 0.0f)
		{
			return true;
		}
		return false;
	}
};

typedef MCircle2<float> Circle2;

#endif