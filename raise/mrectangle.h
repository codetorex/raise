#ifndef MRECTANGLE_H
#define MRECTANGLE_H

#include "mvector2.h"
#include "msize2.h"
#include "mpadding2.h"

/**
 * Rectangle is something already 2d. NO need to append 2 to its name right?
 */
template <class T>
class MRectangle: public MVector2<T>, public MSize2<T>
{
public:
	inline MRectangle()
	{

	}

	inline MRectangle(T rx,T ry, T rw, T rh)
	{
		SetRectangle(rx,ry,rw,rh);
	}

	inline void SetRectangle(T rx,T ry, T rw, T rh)
	{
		X = rx;
		Y = ry;
		Width = rw;
		Height = rh;
	}

	inline void MoveX( T x_)
	{
		X = x_;
	}

	inline void MoveY( T y_)
	{
		Y = y_;
	}

	inline void Move(T x_, T y_)
	{
		X = x_;
		Y = y_;
	}

	inline void ChangeWidth( T w_ )
	{
		Width = w_;
	}

	inline void ChangeHeight( T h_ )
	{
		Height = h_;
	}

	inline void SetRectangle( const MRectangle<T>& other)
	{
		X = other.X;
		Y = other.Y;
		Width = other.Width;
		Height = other.Height;
	}

	inline void SetFromPadding(T totalWidth, T totalHeight, const MPadding2<T>& padding)
	{
		X = padding.Left;
		Y = padding.Top;
		Width = padding.RemainingWidth(totalWidth);
		Height = padding.RemainingHeight(totalHeight);
	}

	inline T Right() const
	{
		return X + Width;
	}

	inline T Bottom() const
	{
		return Y + Height;
	}

	inline void Shrink(T amount)
	{
		X += amount;
		Y += amount;
		Width -= amount * 2;
		Height -= amount * 2;
	}

	inline void Shrink( T shWidth, T shHeight)
	{
		X += shWidth;
		Y += shHeight;
		Width -= shWidth * 2;
		Height -= shHeight * 2;
	}

	inline  void Shrink( const MPadding2<T>& pad )
	{
		X += pad.Left;
		Y += pad.Top;
		Width -= pad.Left + pad.Right;
		Height -= pad.Top + pad.Bottom;
	}

	inline void Inflate( const MPadding2<T>& pad)
	{
		X -= pad.Left;
		Y -= pad.Top;
		Width += pad.Left + pad.Right;
		Height += pad.Top + pad.Bottom;
	}

	inline void Inflate( T size)
	{
		X -= size;
		Y -= size;
		Width += size * 2;
		Height += size * 2;
	}

	inline void Inflate( T infWidth, T infHeight)
	{
		X -= infWidth;
		Y -= infHeight;
		Width += infWidth * 2;
		Height += infHeight * 2;
	}
};

typedef MRectangle<int> IRectangle;

#endif