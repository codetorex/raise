#ifndef VCOLOR_H
#define VCOLOR_H

#include "raisetypes.h"
#include "mtools.h"
#include <string.h>

class TColor32;
class TColorHSL;

class TColors
{
public:

	enum colorss
	{
		aliceblue = 0xfff0f8ff,
		antiquewhite = 0xfffaebd7,
		aqua = 0xff00ffff,
		aquamarine = 0xff7fffd4,
		azure = 0xfff0ffff,
		beige = 0xfff5f5dc,
		bisque = 0xffffe4c4,
		black = 0xff000000,
		blanchedalmond = 0xffffebcd,
		blue = 0xff0000ff,
		blueviolet = 0xff8a2be2,
		brown = 0xffa52a2a,
		burlywood = 0xffdeb887,
		cadetblue = 0xff5f9ea0,
		chartreuse = 0xff7fff00,
		chocolate = 0xffd2691e,
		coral = 0xffff7f50,
		cornflowerblue = 0xff6495ed,
		cornsilk = 0xfffff8dc,
		crimson = 0xffdc143c,
		cyan = 0xff00ffff,
		darkblue = 0xff00008b,
		darkcyan = 0xff008b8b,
		darkgoldenrod = 0xffb8860b,
		darkgray = 0xffa9a9a9,
		darkgreen = 0xff006400,
		darkgrey = 0xffa9a9a9,
		darkkhaki = 0xffbdb76b,
		darkmagenta = 0xff8b008b,
		darkolivegreen = 0xff556b2f,
		darkorange = 0xffff8c00,
		darkorchid = 0xff9932cc,
		darkred = 0xff8b0000,
		darksalmon = 0xffe9967a,
		darkseagreen = 0xff8fbc8f,
		darkslateblue = 0xff483d8b,
		darkslategray = 0xff2f4f4f,
		darkslategrey = 0xff2f4f4f,
		darkturquoise = 0xff00ced1,
		darkviolet = 0xff9400d3,
		deeppink = 0xffff1493,
		deepskyblue = 0xff00bfff,
		dimgray = 0xff696969,
		dimgrey = 0xff696969,
		dodgerblue = 0xff1e90ff,
		firebrick = 0xffb22222,
		floralwhite = 0xfffffaf0,
		forestgreen = 0xff228b22,
		fuchsia = 0xffff00ff,
		gainsboro = 0xffdcdcdc,
		ghostwhite = 0xfff8f8ff,
		gold = 0xffffd700,
		goldenrod = 0xffdaa520,
		gray = 0xff808080,
		green = 0xff008000,
		greenyellow = 0xffadff2f,
		grey = 0xff808080,
		honeydew = 0xfff0fff0,
		hotpink = 0xffff69b4,
		indianred = 0xffcd5c5c,
		indigo = 0xff4b0082,
		ivory = 0xfffffff0,
		khaki = 0xfff0e68c,
		lavender = 0xffe6e6fa,
		lavenderblush = 0xfffff0f5,
		lawngreen = 0xff7cfc00,
		lemonchiffon = 0xfffffacd,
		lightblue = 0xffadd8e6,
		lightcoral = 0xfff08080,
		lightcyan = 0xffe0ffff,
		lightgoldenrodyellow = 0xfffafad2,
		lightgray = 0xffd3d3d3,
		lightgreen = 0xff90ee90,
		lightgrey = 0xffd3d3d3,
		lightpink = 0xffffb6c1,
		lightsalmon = 0xffffa07a,
		lightseagreen = 0xff20b2aa,
		lightskyblue = 0xff87cefa,
		lightslategray = 0xff778899,
		lightslategrey = 0xff778899,
		lightsteelblue = 0xffb0c4de,
		lightyellow = 0xffffffe0,
		lime = 0xff00ff00,
		limegreen = 0xff32cd32,
		linen = 0xfffaf0e6,
		magenta = 0xffff00ff,
		maroon = 0xff800000,
		mediumaquamarine = 0xff66cdaa,
		mediumblue = 0xff0000cd,
		mediumorchid = 0xffba55d3,
		mediumpurple = 0xff9370db,
		mediumseagreen = 0xff3cb371,
		mediumslateblue = 0xff7b68ee,
		mediumspringgreen = 0xff00fa9a,
		mediumturquoise = 0xff48d1cc,
		mediumvioletred = 0xffc71585,
		midnightblue = 0xff191970,
		mintcream = 0xfff5fffa,
		mistyrose = 0xffffe4e1,
		moccasin = 0xffffe4b5,
		navajowhite = 0xffffdead,
		navy = 0xff000080,
		oldlace = 0xfffdf5e6,
		olive = 0xff808000,
		olivedrab = 0xff6b8e23,
		orange = 0xffffa500,
		orangered = 0xffff4500,
		orchid = 0xffda70d6,
		palegoldenrod = 0xffeee8aa,
		palegreen = 0xff98fb98,
		paleturquoise = 0xffafeeee,
		palevioletred = 0xffdb7093,
		papayawhip = 0xffffefd5,
		peachpuff = 0xffffdab9,
		peru = 0xffcd853f,
		pink = 0xffffc0cb,
		plum = 0xffdda0dd,
		powderblue = 0xffb0e0e6,
		purple = 0xff800080,
		red = 0xffff0000,
		rosybrown = 0xffbc8f8f,
		royalblue = 0xff4169e1,
		saddlebrown = 0xff8b4513,
		salmon = 0xfffa8072,
		sandybrown = 0xfff4a460,
		seagreen = 0xff2e8b57,
		seashell = 0xfffff5ee,
		sienna = 0xffa0522d,
		silver = 0xffc0c0c0,
		skyblue = 0xff87ceeb,
		slateblue = 0xff6a5acd,
		slategray = 0xff708090,
		slategrey = 0xff708090,
		snow = 0xfffffafa,
		springgreen = 0xff00ff7f,
		steelblue = 0xff4682b4,
		tan = 0xffd2b48c,
		teal = 0xff008080,
		thistle = 0xffd8bfd8,
		tomato = 0xffff6347,
		turquoise = 0xff40e0d0,
		violet = 0xffee82ee,
		wheat = 0xfff5deb3,
		white = 0xffffffff,
		whitesmoke = 0xfff5f5f5,
		yellow = 0xffffff00,
		yellowgreen = 0xff9acd32,
	};
};

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
* 32 bit color class with ARGB ordering
* In sequential reading it appears like BGRA but when it read as DWORD its ARGB
* bclr 0=b, 1=g, 2=r,3=a
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

	/**
	 * Alpha will be 255.
	 */
	inline void Set24Bit(ui32 color24)
	{
		color = color24;
		a = 255;
	}

	inline void Set24Bit(ui32 color24, byte _a)
	{
		color = color24;
		a = _a;
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

	TColor32 BlendNormal(const TColor32& Destination)
	{
		
	}

	TColor32 BlendMultiply(const TColor32& Destination)
	{

	}

	TColor32 BlendScreen(const TColor32& Destination)
	{

	}

	TColor32 BlendOverlay(const TColor32& Destination)
	{

	}

	TColor32 BlendSoftLight(const TColor32& Destination)
	{

	}

	TColor32 BlendHardLight(const TColor32& Destination)
	{

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

	inline operator byte* (void)
	{
		return bclr;
	}

	inline ui32 GetARGB() const
	{
		ui32 res = (((byte)(b)|((ui16)((byte)(g))<<8))|(((ui32)(byte)(r))<<16)|(((ui32)(byte)(a))<<24));
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

class TColorFloat
{
public:
	float R;
	float G;
	float B;
	float A;

	TColorFloat(float _r,float _g, float _b, float _a)
	{
		R = _r;
		G = _g;
		B = _b;
		A = _a;
	}

	TColorFloat(const TColor32& clr)
	{
		R = (float)clr.r / 255.0f;
		G = (float)clr.g / 255.0f;
		B = (float)clr.b / 255.0f;
		A = (float)clr.a / 255.0f;
	}

	inline TColor32 ConvertToByteColor()
	{
		byte br = (byte)(R * 255.0f);
		byte bg = (byte)(G * 255.0f);
		byte bb = (byte)(B * 255.0f);
		byte ba = (byte)(A * 255.0f);
		return TColor32(br,bg,bb,ba);
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