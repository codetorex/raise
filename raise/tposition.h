#ifndef TPOSITION_H
#define TPOSITION_H

class TPosition
{
public:
	union
	{
		int X;
		int Left;
	};

	union
	{
		int Y;
		int Top;
	};

	inline void SetPosition(int _x, int _y)
	{
		X = _x;
		Y = _y;
	}
};


#endif