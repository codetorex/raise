#ifndef MREGION2_H
#define MREGION2_H

#include "mrectangle.h"

template <class T>
class MRegion2
{
protected:
	MRectangle<T> Rect;
	MVector2<T>	Pos2;

	inline void UpdatePos2X()
	{
		Pos2.X = Rect.X + Rect.Width;
	}

	inline void UpdatePos2Y()
	{
		Pos2.Y = Rect.Y + Rect.Height;
	}

	inline void UpdatePos2()
	{
		UpdatePos2X();
		UpdatePos2Y();
	}

public:

	inline T Width() const
	{
		return Rect.Width;
	}

	inline T Height() const
	{
		return Rect.Height;
	}

	inline T X() const
	{
		return Rect.X;
	}

	inline T Y() const
	{
		return Rect.Y;
	}

	inline T Left() const
	{
		return X();
	}

	inline T Top() const
	{
		return Y();
	}

	inline T Right() const
	{
		return Pos2.X;
	}

	inline T Bottom() const
	{
		return Pos2.Y;
	}

	inline void SetRegion( T x, T y, T w, T h)
	{
		Rect.SetRectangle(x,y,w,h);
		UpdatePos2();
	}

	inline void SetRegion( const MRegion2<T>& other)
	{
		Rect = other.Rect;
		Pos2 = other.Pos2;
	}

	inline void SetRegion( const MRectangle<T>& pRect)
	{
		Rect = pRect;
		UpdatePos2();
	}

	inline void Shrink(const MPadding2<T>& pad)
	{
		Rect.Shrink(pad);
		UpdatePos2();
	}

	inline void Inflate(const MPadding2<T>& pad)
	{
		Rect.Inflate(pad);
		UpdatePos2();
	}

	inline void Inflate(T size)
	{
		Rect.Inflate(size);
		UpdatePos2();
	}

	inline void Inflate(T infWidth , T infHeight)
	{
		Rect.Inflate(infWidth,infHeight);
		UpdatePos2();
	}

	inline void ChangeWidth( T newWidth )
	{
		Rect.Width = newWidth;
		UpdatePos2X();
	}

	inline void ChangeHeight( T newHeight )
	{
		Rect.Height = newHeight;
		UpdatePos2Y();
	}

	inline void ChangeHeightDiff( T heightDiff )
	{
		Rect.Height += heightDiff;
		UpdatePos2Y();
	}

	inline void ChangeWidthDiff( T widthDiff )
	{
		Rect.Width += widthDiff;
		UpdatePos2X();
	}

	inline void MoveXDiff( T xDiff)
	{
		Rect.X += xDiff;
		UpdatePos2X();
	}

	inline void MoveYDiff( T yDiff )
	{
		Rect.Y += yDiff;
		UpdatePos2Y();
	}

	inline void MoveX( T x )
	{
		Rect.X = x;
		UpdatePos2X();
	}
	
	inline void MoveY ( T y )
	{
		Rect.Y = y;
		UpdatePos2Y();
	}

	inline void Move( T x, T y )
	{
		Rect.SetVector(x,y);
		UpdatePos2();
	}


	inline bool IsInside( T x , T y )
	{
		if ( x >= Rect.X && x <= Pos2.X)
		{
			if (y >= Rect.Y && y <= Pos2.Y)
			{
				return true;
			}
		}
		return false;
	}

	operator const MRectangle<T>& (void) const
	{
		return Rect;
	}
};

typedef MRegion2<int> IRegion;


#endif