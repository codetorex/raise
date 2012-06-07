#ifndef MRECTANGLE_H
#define MRECTANGLE_H

#include "mvector2.h"
#include "msize2.h"
#include "mpadding2.h"


enum Alignment
{
	CA_TopLeft,
	CA_TopCenter,
	CA_TopRight,
	CA_MiddleLeft,
	CA_MiddleCenter,
	CA_MiddleRight,
	CA_BottomLeft,
	CA_BottomCenter,
	CA_BottomRight,
};

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

	/**
	 * Aligns a rectangle inside this rectangle
	 */
	void Align(MRectangle<T>& srcRect, Alignment alignment) const
	{
		switch(alignment)
		{
		case CA_TopLeft:
			srcRect.X = 0;
			srcRect.Y = 0;
			return;

		case CA_TopCenter:
			srcRect.X = (Width - srcRect.Width) / 2;
			srcRect.Y = 0;

		case CA_TopRight:
			srcRect.X = Width - srcRect.Width;
			srcRect.Y = 0;
			return;

		case CA_MiddleLeft:
			srcRect.X = 0;
			srcRect.Y = (( Height - srcRect.Height ) / 2);
			return;

		case CA_MiddleCenter:
			srcRect.X = ((Width - srcRect.Width) / 2);
			srcRect.Y = ((Height - srcRect.Height ) / 2);
			return;

		case CA_MiddleRight:
			srcRect.X = Width - srcRect.Width;
			srcRect.Y = (( Height - srcRect.Height ) / 2);
			return;

		case CA_BottomLeft:
			srcRect.X = 0;
			srcRect.Y = ( Height ) - srcRect.Height;
			return;

		case CA_BottomCenter:
			srcRect.X = ((Width - srcRect.Width) / 2);
			srcRect.Y = Height - srcRect.Height;
			return;

		case CA_BottomRight:
			srcRect.X = Width - srcRect.Width;
			srcRect.Y = Height - srcRect.Height;
			return;
		}
	}

	/**
	 * Aligns a rectangle outside of this rectangle
	 */
	void AlignOutside(MRectangle<T>& srcRect, Alignment alignment) const
	{
		Align(srcRect,alignment);
		srcRect.TranslateVector(X,Y);
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

	inline T CenterX()
	{
		return X + (Width / 2);
	}

	inline T CenterY()
	{
		return Y + (Height / 2);
	}

	inline bool IsInside(T pX, T pY) const
	{
		if (pX >= X && pX < Right())
		{
			if (pY >= Y && pY < Bottom())
			{
				return true;
			}
		}

		return false;
	}

	inline bool IsColliding(const MRectangle<T>& r2)
	{
		return ! ( r2.X > Right() || r2.Right() < X || r2.Y > Bottom() || r2.Bottom() < Y );
	}
};

typedef MRectangle<int> IRectangle;

#endif