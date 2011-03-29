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
		color = 0x000000FF;
	}

	inline TColor32ARGB( byte _r, byte _g, byte _b, byte _a)
	{
		color = (((byte)(_b)|((word)((byte)(_g))<<8))|(((dword)(byte)(_r))<<16)|(((dword)(byte)(_a))<<24));
	}

	inline TColor32ARGB(byte _r, byte _g, byte _b)
	{
		color = (((byte)(_b)|((word)((byte)(_g))<<8))|(((dword)(byte)(_r))<<16)|(((dword)(byte)(0xFF))<<24));
	}
};

/**
* 32 bit color class.
*/
class TColor32
{
public:
	union
	{
		dword color;
		byte bclr[4];

		struct
		{
			byte r;
			byte g;
			byte b;
			byte a;
		};
	};

	inline TColor32()
	{
		color = 0xFF000000;
	}

	inline TColor32(dword _color)
	{
		color = color;
	}

	inline TColor32( byte _r, byte _g, byte _b, byte _a)
	{
		color = (((byte)(_r)|((word)((byte)(_g))<<8))|(((dword)(byte)(_b))<<16)|(((dword)(byte)(_a))<<24));
	}

	inline TColor32(byte _r, byte _g, byte _b)
	{
		color = (((byte)(_r)|((word)((byte)(_g))<<8))|(((dword)(byte)(_b))<<16)|(((dword)(byte)(0xFF))<<24));
	}

	inline TColor32( const TColor24& value)
	{
		r = value.r;
		g = value.g;
		b = value.b;
	}

	inline void Set(byte _r, byte _g, byte _b, byte _a)
	{
		color = (((byte)(_r)|((word)((byte)(_g))<<8))|(((dword)(byte)(_b))<<16)|(((dword)(byte)(_a))<<24));
	}

	inline void Set(byte _r, byte _g, byte _b)
	{
		color = (((byte)(_r)|((word)((byte)(_g))<<8))|(((dword)(byte)(_b))<<16)|(((dword)(byte)(0xFF))<<24));
	}

	inline TColor32& operator = (const TColor32& value)
	{
		color = value.color;
		return *this;
	}

	inline TColor32& operator = (dword value)
	{
		color = value;
		return *this;
	}

	inline TColor32& operator = (const TColorHSL& value)
	{
		TColorHSL::ToRGB(value.bclr,bclr);
		return *this;
	}

	inline bool operator == (dword value)
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

	inline operator dword (void) const
	{
		return color;
	}

	inline operator TColor32ARGB (void) const
	{
		TColor32ARGB r;
		r.color = GetARGB();
		return r;
	}

	inline dword GetARGB() const
	{
		dword res = (((byte)(b)|((word)((byte)(g))<<8))|(((dword)(byte)(r))<<16)|(((dword)(byte)(a))<<24));
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