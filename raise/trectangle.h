#ifndef TRECTANGLE_H
#define TRECTANGLE_H

#include "tposition.h"
#include "trange.h"


/**
* Rectangle is derived from Position and Range.
*/
class TRectangle: public TPosition, public TRange
{
public:
	inline void SetRectangle(int _x,int _y,int _width, int _height)
	{
		X = _x;
		Y = _y;
		Width = _width;
		Height = _height;
	}

	inline void SetRectangleFrom(TRectangle* othr)
	{
		X = othr->X;
		Y = othr->Y;
		Width = othr->Width;
		Height = othr->Height;
	}
};

#endif