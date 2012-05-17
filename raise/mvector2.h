#ifndef MVECTOR2_H
#define MVECTOR2_H

#include "raisetypes.h"

template <class T>
class MVector2
{
public:
	union
	{
		struct
		{
			T x;
			T y;
		};

		T v[2];
	};

	inline MVector2( const MVector2<T>& other)
	{
		x = other.x;
		y = other.y;
	}

	inline MVector2()
	{
		x = 0;
		y = 0;
	}

	inline MVector2(T x_,T y_)
	{
		x = x_;
		y = y_;
	}

	inline void Set( T x_,T y_ )
	{
		x = x_;
		y = y_;
	}

	inline MVector2<T>& operator += ( const MVector2<T>& value)
	{
		x += value.x;
		y += value.y;
		return *this;
	}

	inline MVector2<T>& operator -= ( const MVector2<T>& value)
	{
		x -= value.x;
		y -= value.y;
		return *this;
	}

	inline MVector2<T> operator + ( const MVector2<T>& value) const
	{
		MVector2<T> result(*this);
		result.x += value.x;
		result.y += value.y;
		return result;
	}

	inline T Distance( const MVector2<T>& other) const
	{
		T dx = other.x - x;
		T dy = other.y - y;
		return MathDriver::SquareRoot( ( dx * dx ) + ( dy * dy ) );
	}

	// TODO: do the rest
};

typedef MVector2<float>		Vector2;
typedef Vector2				Point2;
typedef MVector2<int>		Vector2i;

#endif