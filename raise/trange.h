#ifndef TRANGE_H
#define TRANGE_H

#include "mvector2.h"

class TRange
{
public:
	int Width;
	int Height;

	inline void SetRange( const TRange& other)
	{
		Width = other.Width;
		Height = other.Height;
	}

	inline void SetRange(int _width, int _height)
	{
		Width = _width;
		Height = _height;
	}

	inline float GetXRatio(int x)
	{
		return ((float)x / (float)Width);
	}

	inline float GetYRatio(int y)
	{
		return ((float)y / (float)Height);
	}

	inline int GetXFromRatio(float x)
	{
		return (int)(x * (float)Width);
	}

	inline int GetYFromRatio(float y)
	{
		return (int)(y * (float)Height);
	}

	inline vec2 GetRatio(int x, int y)
	{
		return vec2(GetXRatio(x),GetYRatio(y));
	}

	inline vec2i GetPositionFromRatio(float x, float y)
	{
		return vec2i(GetXFromRatio(x),GetYFromRatio(y));
	}

	inline int GetArea()
	{
		return Width * Height;
	}
};

typedef TRange Size;

#endif