#ifndef MSIZE2_H
#define MSIZE2_H

template <class T>
class MSize2
{
public:
	T Width;
	T Height;

	inline void SetSize(T w, T h)
	{
		Width = w;
		Height = h;
	}

	inline void SetSize( const MSize2<T>& other)
	{
		Width = other.Width;
		Height = other.Height;
	}

	inline float WidthRatio(T x) const
	{
		return ((float)x / (float)Width);
	}

	inline float HeightRatio(T y) const
	{
		return ((float)y / (float)Height);
	}

	Vector2 Ratio(T x,T y) const
	{
		Vector2 result(WidthRatio(x),HeightRatio(y));
		return result;
	}

	inline T XFromRatio(float xratio) const
	{
		return (T)(xratio * (float)Width);
	}

	inline T YFromRatio(float yratio) const
	{
		return (T)(yratio * (float)Height);
	}

	inline MVector2<T> VectorFromRatio(float xratio, float yratio) const
	{
		return MVector2<T>(XFromRatio(xratio),YFromRatio(yratio));
	}

	inline T Area() const
	{
		return (Width * Height);
	}
};

typedef MSize2<int> ISize;
//typedef ISize TRange;

#endif