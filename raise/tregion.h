#ifndef TREGION_H
#define TREGION_H

#include "trectangle.h"

/**
* Region is derived from Rectangle, which is derived from Position and Range.
*/
class TRegion: public TRectangle
{
public:

	TRegion()
	{
		X = 0;
		Y = 0;
		Width = 0;
		Height = 0;
		Right = 0;
		Bottom = 0;
	}

	int Right;
	int Bottom;

	inline void SetWidth(int _width)
	{
		Width = _width;
		Right = X + Width;
	}

	inline void SetHeight(int _height)
	{
		Height = _height;
		Bottom = Y +_height;
	}

	inline void SetLeft(int _left)
	{
		Left = _left;
		Right = Left + Width;
	}

	inline void SetLeftRelative(int _leftDifference)
	{
		SetLeft(Left + _leftDifference);
	}

	inline void SetTopRelative(int _topDifference)
	{
		SetTop(Top + _topDifference);
	}

	inline void SetTop(int _top)
	{
		Top = _top;
		Bottom = Top + Height;
	}

	inline void SetLeftTop(int _left, int _top)
	{
		SetLeft(_left); // x
		SetTop(_top); // y
	}

	inline void SetRight(int _right)
	{
		Right = _right;
		Width = Right - Left;
	}

	inline void SetBottom(int _bottom)
	{
		Bottom = _bottom;
		Height = Bottom - Top;
	}

	inline void SetSize(int _x,int _y, int _width, int _height)
	{
		X = _x;
		Y = _y;
		SetWidth(_width);
		SetHeight(_height);
	}

	inline void SetFrom(TRegion* othr)
	{
		X = othr->X;
		Y = othr->Y;
		Width = othr->Width;
		Height = othr->Height;
		Right = othr->Right;
		Bottom = othr->Bottom;
	}

	inline void SetFrom(TRectangle* othr)
	{
		SetSize(othr->X,othr->Y,othr->Width,othr->Height);
	}

	inline void SetRelative(TRegion* parent,int relativeX,int relativeY)
	{
		X = parent->X + relativeX;
		Y = parent->Y + relativeY;
		SetWidth(parent->Width);
		SetHeight(parent->Height);
	}

	inline void SetRect(int _left,int _top, int _right, int _bottom)
	{
		Left = _left;
		Top = _bottom;
		SetRight(_right);
		SetBottom(_bottom);
	}

	inline bool IsInside(int x,int y)
	{
		if (x >= Left && x <= Right)
		{
			if (y >= Top && y <= Bottom)
			{
				return true;
			}
		}
		return false;
	}
};

#endif

