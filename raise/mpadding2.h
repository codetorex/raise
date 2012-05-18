#ifndef MPADDING2_H
#define MPADDING2_H


template <class T>
class MPadding2
{
public:
	T Left;
	T Top;
	T Right;
	T Bottom;

	MPadding2()
	{
		SetPadding(0);
	}

	MPadding2<T>& operator += (const MPadding2<T>& other)
	{
		Left += other.Left;
		Top += other.Top;
		Right += other.Right;
		Bottom += other.Bottom;
		return *this;
	}

	inline void SetPadding(T pad)
	{
		Left = pad;
		Top = pad;
		Right = pad;
		Bottom = pad;
	}

	inline void SetPadding(T horizontal, T vertical)
	{
		Left = horizontal;
		Right = horizontal;
		Top = vertical;
		Bottom = vertical;
	}

	inline void SetPadding(T pLeft, T pTop, T pRight, T pBottom)
	{
		Left = pLeft;
		Top = pTop;
		Right = pRight;
		Bottom = pBottom;
	}

	inline T RemainingWidth(T totalWidth) const
	{
		return totalWidth - (Left + Right);
	}

	inline T RemainingHeight(T totalHeight) const
	{
		return totalHeight - (Top + Bottom);
	}
};

typedef MPadding2<int> IPadding;

#endif