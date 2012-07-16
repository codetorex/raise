#ifndef VCOLOR_H
#define VCOLOR_H

#include "raisetypes.h"
#include "mtools.h"
#include <string.h>

class TColor32RGBA;
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

	inline TColor24& operator = (const TColor32RGBA& value);

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

	inline TColorHSL ( const TColor32RGBA& value);

	inline TColorHSL& operator = (const TColor24& value)
	{
		ToHSL(value.bclr,bclr);
		return *this;
	}

	inline TColorHSL& operator = (const TColor32RGBA& value);

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
* 32 bit color class with RGBA ordering
* In sequential reading it appears like RGBA but when it read as DWORD its ABGR cuz its little endian
* bclr 0=r, 1=g, 2=b,3=a
*/
class TColor32RGBA
{
public:
	union
	{
		ui32 color;
		byte bclr[4];

		struct
		{
			byte r;
			byte g;
			byte b;
			byte a;
		};
	};

	inline TColor32RGBA()
	{
		color = Black;
	}

	inline TColor32RGBA(ui32 _color)
	{
		color = _color;
	}

	inline TColor32RGBA( byte _r, byte _g, byte _b, byte _a = 255)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	inline TColor32RGBA( const TColor24& value)
	{
		r = value.r;
		g = value.g;
		b = value.b;
		a = 255;
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

	inline TColor32RGBA& operator = (const TColor32RGBA& value)
	{
		color = value.color;
		return *this;
	}

	inline TColor32RGBA& operator = (ui32 value)
	{
		color = value;
		return *this;
	}

	inline TColor32RGBA& operator = (const TColorHSL& value)
	{
		TColorHSL::ToRGB(value.bclr,bclr);
		return *this;
	}

	/* EQUALITY OPERATORS ARE SUPPLIED BY C++ DWORD == DWORD
	 *
	 *inline bool operator == (ui32 value)
	{
		if (color == value)
		{
			return true;
		}
		return false;
	}*/

	/*inline bool operator == (const TColor24& value)
	{
		if ( ((value.r - r) + (value.g - g) + (value.b - b)) == 0 ) // or different implementation
		{
			return true;
		}
		return false;
	}*/

	/**/

	/*inline bool operator == (const TColor32RGBA& value)
	{
		if (value.color == color)
		{
			return true;
		}
		return false;
	}*/

	inline TColor32RGBA& operator += (const TColor32RGBA& value)
	{
		r = caddb(value.r,r);
		g = caddb(value.g,g);
		b = caddb(value.b,b);
		a = caddb(value.a,a);
		return *this;
	}

	inline TColor32RGBA operator + (const TColor32RGBA& value)
	{
		TColor32RGBA kp(caddb(value.r,r),caddb(value.g,g),caddb(value.b,b),caddb(value.a,a));
		return kp;
	}

	inline TColor32RGBA operator + (const TColor24& value)
	{
		TColor32RGBA kp(caddb(value.r,r),caddb(value.g,g),caddb(value.b,b),a);
		return kp;
	}

	inline operator ui32 (void) const
	{
		return color;
	}

	/*inline operator byte* (void)
	{
		return bclr;
	}*/

	/*inline TString ToWebColor()
	{
		return TString::Format("#%%%",sfx(r,2,'0'),sfx(g,2,'0'),sfx(b,2,'0'));
	}

	static TColor32RGBA FromWebColor(const TString& value)
	{

	}*/

	inline void Monochrome()
	{
		int ts = ((int)r + (int)g + (int)b) / 3;
		r = (byte)ts;
		g = r;
		b = r;
	}

	enum RGBAColors
	{
		TestColor = 0xFFB28040,
		AliceBlue = 0xFFFFF8F0,
		AntiqueWhite = 0xFFD7EBFA,
		Aqua = 0xFFFFFF00,
		Aquamarine = 0xFFD4FF7F,
		Azure = 0xFFFFFFF0,
		Beige = 0xFFDCF5F5,
		Bisque = 0xFFC4E4FF,
		Black = 0xFF000000,
		BlanchedAlmond = 0xFFCDEBFF,
		Blue = 0xFFFF0000,
		BlueViolet = 0xFFE22B8A,
		Brown = 0xFF2A2AA5,
		BurlyWood = 0xFF87B8DE,
		CadetBlue = 0xFFA09E5F,
		Chartreuse = 0xFF00FF7F,
		Chocolate = 0xFF1E69D2,
		Coral = 0xFF507FFF,
		CornflowerBlue = 0xFFED9564,
		Cornsilk = 0xFFDCF8FF,
		Crimson = 0xFF3C14DC,
		Cyan = 0xFFFFFF00,
		DarkBlue = 0xFF8B0000,
		DarkCyan = 0xFF8B8B00,
		DarkGoldenRod = 0xFF0B86B8,
		DarkGray = 0xFFA9A9A9,
		DarkGrey = 0xFFA9A9A9,
		DarkGreen = 0xFF006400,
		DarkKhaki = 0xFF6BB7BD,
		DarkMagenta = 0xFF8B008B,
		DarkOliveGreen = 0xFF2F6B55,
		Darkorange = 0xFF008CFF,
		DarkOrchid = 0xFFCC3299,
		DarkRed = 0xFF00008B,
		DarkSalmon = 0xFF7A96E9,
		DarkSeaGreen = 0xFF8FBC8F,
		DarkSlateBlue = 0xFF8B3D48,
		DarkSlateGray = 0xFF4F4F2F,
		DarkSlateGrey = 0xFF4F4F2F,
		DarkTurquoise = 0xFFD1CE00,
		DarkViolet = 0xFFD30094,
		DeepPink = 0xFF9314FF,
		DeepSkyBlue = 0xFFFFBF00,
		DimGray = 0xFF696969,
		DimGrey = 0xFF696969,
		DodgerBlue = 0xFFFF901E,
		FireBrick = 0xFF2222B2,
		FloralWhite = 0xFFF0FAFF,
		ForestGreen = 0xFF228B22,
		Fuchsia = 0xFFFF00FF,
		Gainsboro = 0xFFDCDCDC,
		GhostWhite = 0xFFFFF8F8,
		Gold = 0xFF00D7FF,
		GoldenRod = 0xFF20A5DA,
		Gray = 0xFF808080,
		Grey = 0xFF808080,
		Green = 0xFF008000,
		GreenYellow = 0xFF2FFFAD,
		HoneyDew = 0xFFF0FFF0,
		HotPink = 0xFFB469FF,
		IndianRed  = 0xFF5C5CCD,
		Indigo  = 0xFF82004B,
		Ivory = 0xFFF0FFFF,
		Khaki = 0xFF8CE6F0,
		Lavender = 0xFFFAE6E6,
		LavenderBlush = 0xFFF5F0FF,
		LawnGreen = 0xFF00FC7C,
		LemonChiffon = 0xFFCDFAFF,
		LightBlue = 0xFFE6D8AD,
		LightCoral = 0xFF8080F0,
		LightCyan = 0xFFFFFFE0,
		LightGoldenRodYellow = 0xFFD2FAFA,
		LightGray = 0xFFD3D3D3,
		LightGrey = 0xFFD3D3D3,
		LightGreen = 0xFF90EE90,
		LightPink = 0xFFC1B6FF,
		LightSalmon = 0xFF7AA0FF,
		LightSeaGreen = 0xFFAAB220,
		LightSkyBlue = 0xFFFACE87,
		LightSlateGray = 0xFF998877,
		LightSlateGrey = 0xFF998877,
		LightSteelBlue = 0xFFDEC4B0,
		LightYellow = 0xFFE0FFFF,
		Lime = 0xFF00FF00,
		LimeGreen = 0xFF32CD32,
		Linen = 0xFFE6F0FA,
		Magenta = 0xFFFF00FF,
		Maroon = 0xFF000080,
		MediumAquaMarine = 0xFFAACD66,
		MediumBlue = 0xFFCD0000,
		MediumOrchid = 0xFFD355BA,
		MediumPurple = 0xFFD87093,
		MediumSeaGreen = 0xFF71B33C,
		MediumSlateBlue = 0xFFEE687B,
		MediumSpringGreen = 0xFF9AFA00,
		MediumTurquoise = 0xFFCCD148,
		MediumVioletRed = 0xFF8515C7,
		MidnightBlue = 0xFF701919,
		MintCream = 0xFFFAFFF5,
		MistyRose = 0xFFE1E4FF,
		Moccasin = 0xFFB5E4FF,
		NavajoWhite = 0xFFADDEFF,
		Navy = 0xFF800000,
		OldLace = 0xFFE6F5FD,
		Olive = 0xFF008080,
		OliveDrab = 0xFF238E6B,
		Orange = 0xFF00A5FF,
		OrangeRed = 0xFF0045FF,
		Orchid = 0xFFD670DA,
		PaleGoldenRod = 0xFFAAE8EE,
		PaleGreen = 0xFF98FB98,
		PaleTurquoise = 0xFFEEEEAF,
		PaleVioletRed = 0xFF9370D8,
		PapayaWhip = 0xFFD5EFFF,
		PeachPuff = 0xFFB9DAFF,
		Peru = 0xFF3F85CD,
		Pink = 0xFFCBC0FF,
		Plum = 0xFFDDA0DD,
		PowderBlue = 0xFFE6E0B0,
		Purple = 0xFF800080,
		Red = 0xFF0000FF,
		RosyBrown = 0xFF8F8FBC,
		RoyalBlue = 0xFFE16941,
		SaddleBrown = 0xFF13458B,
		Salmon = 0xFF7280FA,
		SandyBrown = 0xFF60A4F4,
		SeaGreen = 0xFF578B2E,
		SeaShell = 0xFFEEF5FF,
		Sienna = 0xFF2D52A0,
		Silver = 0xFFC0C0C0,
		SkyBlue = 0xFFEBCE87,
		SlateBlue = 0xFFCD5A6A,
		SlateGray = 0xFF908070,
		SlateGrey = 0xFF908070,
		Snow = 0xFFFAFAFF,
		SpringGreen = 0xFF7FFF00,
		SteelBlue = 0xFFB48246,
		Tan = 0xFF8CB4D2,
		Teal = 0xFF808000,
		Thistle = 0xFFD8BFD8,
		Tomato = 0xFF4763FF,
		Turquoise = 0xFFD0E040,
		Violet = 0xFFEE82EE,
		Wheat = 0xFFB3DEF5,
		White = 0xFFFFFFFF,
		WhiteSmoke = 0xFFF5F5F5,
		Yellow = 0xFF00FFFF,
		YellowGreen = 0xFF32CD9A,
	};
};

typedef TColor32RGBA TColor32;
typedef TColor32 TColors;

/**
* 32 bit color class with BGRA ordering
* In sequential reading it appears like BGRA but when it read as DWORD its ARGB cuz its little endian
* bclr 0=b, 1=g, 2=r,3=a
*/
class TColor32BGRA
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

	inline TColor32BGRA()
	{
		color = 0;
		a = 255;
	}

	inline TColor32BGRA(const TColor32RGBA& clr)
	{
		b = clr.b;
		g = clr.g;
		r = clr.r;
		a = clr.a;
	}

	inline bool operator == (const TColor32RGBA& value)
	{
		if (value.r == r && value.g == g && value.b == b && value.a == a)
		{
			return true;
		}
		return false;
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

	TColorFloat(const TColor32RGBA& clr)
	{
		R = (float)clr.r / 255.0f;
		G = (float)clr.g / 255.0f;
		B = (float)clr.b / 255.0f;
		A = (float)clr.a / 255.0f;
	}

	inline TColor32RGBA ConvertToByteColor()
	{
		byte br = (byte)(R * 255.0f);
		byte bg = (byte)(G * 255.0f);
		byte bb = (byte)(B * 255.0f);
		byte ba = (byte)(A * 255.0f);
		return TColor32RGBA(br,bg,bb,ba);
	}
};

inline TColorHSL::TColorHSL(const TColor32RGBA& value)
{
	ToHSL(value.bclr,bclr);
}

/*TColorHSL::TColorHSL ( const TColor32& value)
{
	ToHSL(value.bclr,bclr);
}*/

inline TColorHSL& TColorHSL::operator = (const TColor32RGBA& value)
{
	ToHSL(value.bclr,bclr);
	return *this;
}


inline TColor24& TColor24::operator = (const TColor32RGBA& value)
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