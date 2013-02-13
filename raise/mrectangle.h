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
		this->X = rx;
		this->Y = ry;
		this->Width = rw;
		this->Height = rh;
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
			srcRect.X = (this->Width - srcRect.Width) / 2;
			srcRect.Y = 0;

		case CA_TopRight:
			srcRect.X = this->Width - srcRect.Width;
			srcRect.Y = 0;
			return;

		case CA_MiddleLeft:
			srcRect.X = 0;
			srcRect.Y = (( this->Height - srcRect.Height ) / 2);
			return;

		case CA_MiddleCenter:
			srcRect.X = ((this->Width - srcRect.Width) / 2);
			srcRect.Y = ((this->Height - srcRect.Height ) / 2);
			return;

		case CA_MiddleRight:
			srcRect.X = this->Width - srcRect.Width;
			srcRect.Y = (( this->Height - srcRect.Height ) / 2);
			return;

		case CA_BottomLeft:
			srcRect.X = 0;
			srcRect.Y = ( this->Height ) - srcRect.Height;
			return;

		case CA_BottomCenter:
			srcRect.X = ((this->Width - srcRect.Width) / 2);
			srcRect.Y = this->Height - srcRect.Height;
			return;

		case CA_BottomRight:
			srcRect.X = this->Width - srcRect.Width;
			srcRect.Y = this->Height - srcRect.Height;
			return;
		}
	}

	/**
	 * Aligns a rectangle outside of this rectangle
	 */
	void AlignOutside(MRectangle<T>& srcRect, Alignment alignment) const
	{
		Align(srcRect,alignment);
		srcRect.TranslateVector(this->X,this->Y);
	}

	inline void MoveX( T x_)
	{
		this->X = x_;
	}

	inline void MoveY( T y_)
	{
		this->Y = y_;
	}

	inline void Move(T x_, T y_)
	{
		this->X = x_;
		this->Y = y_;
	}

	inline void ChangeWidth( T w_ )
	{
		this->Width = w_;
	}

	inline void ChangeHeight( T h_ )
	{
		this->Height = h_;
	}

	inline void SetRectangle( const MRectangle<T>& other)
	{
		this->X = other.X;
		this->Y = other.Y;
		this->Width = other.Width;
		this->Height = other.Height;
	}

	inline void SetFromPadding(T totalWidth, T totalHeight, const MPadding2<T>& padding)
	{
		this->X = padding.Left;
		this->Y = padding.Top;
		this->Width = padding.RemainingWidth(totalWidth);
		this->Height = padding.RemainingHeight(totalHeight);
	}

	inline T Right() const
	{
		return this->X + this->Width;
	}

	inline T Bottom() const
	{
		return this->Y + this->Height;
	}

	inline void Shrink(T amount)
	{
		this->X += amount;
		this->Y += amount;
		this->Width -= amount * 2;
		this->Height -= amount * 2;
	}

	inline void Shrink( T shWidth, T shHeight)
	{
		this->X += shWidth;
		this->Y += shHeight;
		this->Width -= shWidth * 2;
		this->Height -= shHeight * 2;
	}

	inline  void Shrink( const MPadding2<T>& pad )
	{
		this->X += pad.Left;
		this->Y += pad.Top;
		this->Width -= pad.Left + pad.Right;
		this->Height -= pad.Top + pad.Bottom;
	}

	inline void Inflate( const MPadding2<T>& pad)
	{
		this->X -= pad.Left;
		this->Y -= pad.Top;
		this->Width += pad.Left + pad.Right;
		this->Height += pad.Top + pad.Bottom;
	}

	inline void Inflate( T size)
	{
		this->X -= size;
		this->Y -= size;
		this->Width += size * 2;
		this->Height += size * 2;
	}

	inline void Inflate( T infWidth, T infHeight)
	{
		this->X -= infWidth;
		this->Y -= infHeight;
		this->Width += infWidth * 2;
		this->Height += infHeight * 2;
	}

	inline T CenterX()
	{
		return this->X + (this->Width / 2);
	}

	inline T CenterY()
	{
		return this->Y + (this->Height / 2);
	}

	inline bool IsInside(T pX, T pY) const
	{
		if (pX >= this->X && pX < Right())
		{
			if (pY >= this->Y && pY < Bottom())
			{
				return true;
			}
		}

		return false;
	}

	inline bool IsColliding(const MRectangle<T>& r2)
	{
		return ! ( r2.X > Right() || r2.Right() < this->X || r2.Y > Bottom() || r2.Bottom() < this->Y );
	}
};

typedef MRectangle<int> IRectangle;

#endif