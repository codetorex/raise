#ifndef VCOLOR_H
#define VCOLOR_H

#include "raisetypes.h"
#include "mtools.h"
#include <string.h>

class TColor32;
class TColorHSL;

class TColor24
{
public:
	union
	{
		byte bclr[3];

		struct
		{
			byte r;
			byte g;
			byte b;
		};
	};

	inline TColor24()
	{
		r = 0;
		g = 0;
		b = 0;
	}

	inline TColor24(byte _r, byte _g, byte _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}

	inline TColor24& operator = (const TColor32& value);

	inline TColor24& operator = (const byte* value)
	{
		r = value[0];
		g = value[1];
		b = value[2];
	}

	inline TColor24& operator += (const TColor24& value)
	{
		r = caddb(value.r,r);
		g = caddb(value.g,g);
		b = caddb(value.b,b);
		return *this;
	}

	inline TColor24 operator + (const TColor24& value)
	{
		TColor24 kp(caddb(value.r,r),caddb(value.g,g),caddb(value.b,b));
		return kp;
	}

	inline void mono()
	{
		int ts = ((int)r + (int)g + (int)b) / 3;
		r = (byte)ts;
		g = r;
		b = r;
	}

	inline TColor24& operator = (const TColorHSL& value);


};

/**
* Color class that represented by Hue ,Saturation and Luminance
*/
class TColorHSL
{
public:
	union
	{
		byte bclr[3];

		struct
		{
			byte h;
			byte s;
			byte l;
		};
	};

	inline TColorHSL ( const TColor24& value)
	{
		ToHSL(value.bclr,bclr);
	}

	inline TColorHSL ( const TColor32& value);

	inline TColorHSL& operator = (const TColor24& value)
	{
		ToHSL(value.bclr,bclr);
		return *this;
	}

	inline TColorHSL& operator = (const TColor32& value);

	/**
	* This function converts RGB byte array to HSL byte array.
	*/
	static void ToHSL(const byte* rgb,byte* hsl);

	/**
	* This function converts HSL byte array to RGB byte array.
	*/
	static void ToRGB(const byte* hsl,byte* rgb);
};

/**
* 32 bit color class with RGBA ordering.
*/
class TColor32
{
public:
	union
	{
		ui32 color;
		byte bclr[4];

		struct
		{
			byte b;
			byte g;
			byte r;
			byte a;
		};
	};

	inline TColor32()
	{
		color = 0xFF000000;
	}

	inline TColor32(ui32 _color)
	{
		color = _color;
	}

	inline TColor32( byte _r, byte _g, byte _b, byte _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	inline TColor32(byte _r, byte _g, byte _b)
	{
		r = _r;
		g = _g;
		b = _b;
		a = 255;
	}

	inline TColor32( const TColor24& value)
	{
		r = value.r;
		g = value.g;
		b = value.b;
	}

	inline void Set(byte _r, byte _g, byte _b, byte _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	inline void Set(byte _r, byte _g, byte _b)
	{
		r = _r;
		g = _g;
		b = _b;
		a = 255;
	}

	inline TColor32& operator = (const TColor32& value)
	{
		color = value.color;
		return *this;
	}

	inline TColor32& operator = (ui32 value)
	{
		color = value;
		return *this;
	}

	inline TColor32& operator = (const TColorHSL& value)
	{
		TColorHSL::ToRGB(value.bclr,bclr);
		return *this;
	}

	inline bool operator == (ui32 value)
	{
		if (color == value)
		{
			return true;
		}
		return false;
	}

	inline bool operator == (const TColor24& value)
	{
		if ( ((value.r - r) + (value.g - g) + (value.b - b)) == 0 ) // or different implementation
		{
			return true;
		}
		return false;
	}

	inline bool operator == (const TColor32& value)
	{
		if (value.color == color)
		{
			return true;
		}
		return false;
	}

	inline TColor32& operator += (const TColor32& value)
	{
		r = caddb(value.r,r);
		g = caddb(value.g,g);
		b = caddb(value.b,b);
		a = caddb(value.a,a);
		return *this;
	}

	inline TColor32 operator + (const TColor32& value)
	{
		TColor32 kp(caddb(value.r,r),caddb(value.g,g),caddb(value.b,b),caddb(value.a,a));
		return kp;
	}

	inline TColor32 operator + (const TColor24& value)
	{
		TColor32 kp(caddb(value.r,r),caddb(value.g,g),caddb(value.b,b),a);
		return kp;
	}

	inline operator ui32 (void) const
	{
		return color;
	}

	inline ui32 GetARGB() const
	{
		ui32 res = (((byte)(b)|((uint16)((byte)(g))<<8))|(((ui32)(byte)(r))<<16)|(((ui32)(byte)(a))<<24));
		return res;
	}


	inline void mono()
	{
		int ts = ((int)r + (int)g + (int)b) / 3;
		r = (byte)ts;
		g = r;
		b = r;
	}
};

/**
* 32 bit color class with ARGB ordering.
*/
/*
class TColor32ARGB
{
public:
	union
	{
		dword color;
		byte bclr[4];

		struct
		{
			byte a;
			byte r;
			byte g;
			byte b;
		};
	};

	inline TColor32ARGB()
	{
		color = (((byte)(0)|((word)((byte)(0))<<8))|(((dword)(byte)(0))<<16)|(((dword)(byte)(255))<<24));
	}

	inline TColor32ARGB(dword _color)
	{
		color = _color;
	}

	inline TColor32ARGB( byte _r, byte _g, byte _b, byte _a)
	{
		color = (((byte)(_b)|((word)((byte)(_g))<<8))|(((dword)(byte)(_r))<<16)|(((dword)(byte)(_a))<<24));
	}

	inline TColor32ARGB(byte _r, byte _g, byte _b)
	{
		color = (((byte)(_b)|((word)((byte)(_g))<<8))|(((dword)(byte)(_r))<<16)|(((dword)(byte)(0xFF))<<24));
	}

	inline TColor32ARGB(const TColor32& othr)
	{
		//color = (othr.color << 8) | (othr.color >> 24);
		color = (((byte)(othr.b)|((word)((byte)(othr.g))<<8))|(((dword)(byte)(othr.r))<<16)|(((dword)(byte)(othr.a))<<24));
	}

	inline TColor32ARGB& operator = (const TColor32& othr)
	{
		color = (((byte)(othr.b)|((word)((byte)(othr.g))<<8))|(((dword)(byte)(othr.r))<<16)|(((dword)(byte)(othr.a))<<24));
		return *this;
	}


	inline operator dword (void) const
	{
		return color;
	}
};
*/

inline TColorHSL::TColorHSL(const TColor32& value)
{
	ToHSL(value.bclr,bclr);
}

/*TColorHSL::TColorHSL ( const TColor32& value)
{
	ToHSL(value.bclr,bclr);
}*/

inline TColorHSL& TColorHSL::operator = (const TColor32& value)
{
	ToHSL(value.bclr,bclr);
	return *this;
}


inline TColor24& TColor24::operator = (const TColor32& value)
{
	r = value.r;
	g = value.g;
	b = value.b;
	return *this;
}

inline TColor24& TColor24::operator = (const TColorHSL& value)
{
	TColorHSL::ToRGB(value.bclr,bclr);
	return *this;
}




#endif