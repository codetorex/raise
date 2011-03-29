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
			T x,y;
		};

		T v[2];
	};

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

	inline void set( T x_,T y_ )
	{
		x = x_;
		y = y_;
	}

	// TODO: do the rest
};

typedef MVector2<float>		vec2;
typedef vec2 point2;
typedef MVector2<int>		vec2i;

#endif