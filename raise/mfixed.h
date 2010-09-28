#ifndef MFIXEDPOINT_H
#define MFIXEDPOINT_H

#include "raisetypes.h"

// Do people still using fixed points nowadays?
// But implemented it for LOLZ
// TODO: if you ever try to use this, change this to template

// 28.4 fixed point
class RDLL MFixedPoint
{
public:
	MFixedPoint()
	{
		a = 0;
	}

	MFixedPoint(float b)
	{
		*this = b;
	}

	MFixedPoint(int b)
	{
		*this = b;
	}

	int a;

	operator float (void)
	{
		return ((float)a / 16.0f);
	}

	operator int (void)
	{
		return (a >> 4);
	}

	MFixedPoint& operator =  (float b)
	{
		int ri;
		b *= 16.0f;

#ifndef LINUX
		__asm
		{
			fld  b
			fistp ri
		}
#else
		asm(".intel_syntax noprefix\n");
		asm("fld  b\n");
		asm("fistp ri\n");
#endif

		a = ri;
		return *this;
	}

	MFixedPoint& operator =  (int b)
	{
		a = b << 4;
		return *this;
	}

	MFixedPoint operator+ (MFixedPoint b)
	{
		MFixedPoint c;
		c.a = a + b.a;
		return c;
	}

	MFixedPoint operator- (MFixedPoint b)
	{
		MFixedPoint c;
		c.a = a - b.a;
		return c;	
	}

	MFixedPoint	operator* (MFixedPoint b)
	{
		MFixedPoint c;
		c.a = ((a) * (b.a)) >> 4;
		return c;
	}

	MFixedPoint operator/ (MFixedPoint b)
	{
		MFixedPoint c;
		c.a = ((a << 4) / b.a);
		return c;
	}
};

typedef MFixedPoint fxd;

#endif