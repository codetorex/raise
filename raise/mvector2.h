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
			T X;
			T Y;
		};

		T v[2];
	};

	inline MVector2( const MVector2<T>& other)
	{
		X = other.X;
		Y = other.Y;
	}

	inline MVector2()
	{
		X = 0;
		Y = 0;
	}

	inline MVector2(T x_,T y_)
	{
		X = x_;
		Y = y_;
	}

	inline void SetVector( T x_,T y_ )
	{
		X = x_;
		Y = y_;
	}

	inline void TranslateVector( T x_, T y_)
	{
		X += x_;
		Y += y_;
	}

	inline void SubtractVector(T x_, T y_)
	{
		X -= x_;
		Y -= y_;
	}

	inline MVector2<T>& operator += ( const MVector2<T>& value)
	{
		X += value.X;
		Y += value.Y;
		return *this;
	}

	inline MVector2<T>& operator -= ( const MVector2<T>& value)
	{
		X -= value.X;
		Y -= value.Y;
		return *this;
	}

	inline MVector2<T> operator + ( const MVector2<T>& value) const
	{
		MVector2<T> result(*this);
		result.X += value.X;
		result.Y += value.Y;
		return result;
	}

	inline T Distance( const MVector2<T>& other) const
	{
		T dx = other.X - X;
		T dy = other.Y - Y;
		return MathDriver::SquareRoot( ( dx * dx ) + ( dy * dy ) );
	}

	// TODO: do the rest
};

typedef MVector2<float>		Vector2;
typedef Vector2				Point2;
typedef MVector2<int>		IPosition;
typedef IPosition			IVector2;

#endif