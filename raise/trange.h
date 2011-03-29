#ifndef TRANGE_H
#define TRANGE_H

#include "mvector2.h"

class TRange
{
public:
	int Width;
	int Height;

	inline void SetRange(int _width, int _height)
	{
		Width = _width;
		Height = _height;
	}

	inline float GetXRatio(float x)
	{
		return (x / (float)Width);
	}

	inline float GetYRatio(float y)
	{
		return (y / (float)Height);
	}

	inline vec2 GetRatio(float x, float y)
	{
		return vec2(GetXRatio(x),GetYRatio(y));
	}

	inline int GetArea()
	{
		return Width * Height;
	}
};

#endif