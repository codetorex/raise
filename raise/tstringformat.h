#ifndef TSTRINGFORMAT_H
#define TSTRINGFORMAT_H

#include "tconvert.h"

class TString;

/// I implemented two different system here...
/// This is first one, one class holds all data necessary, and string builder takes care of
/// Value to string conversion and appending operation

enum TSign
{
	SGN_NEG,
	SGN_BOTH, // writes positive signs too
	SGN_NONE,
};

enum TElementType
{
	F_INT			= 'i',
	F_UINT			= 'u',
	F_FLOAT			= 'f',
	F_HEX			= 'x',
	F_HEXCAPITAL	= 'X',
	F_STRING		= 's',
};

class TStringFormatElement
{
public:
	TElementType	Type;
	char			PadChar;
	int				PadWidth; // 0 means no padding, positive numbers pads left side, negative numbers pads right side
	TSign			SignFormat;
	int				Precision; // -1 means full precision

	union
	{
		int			int_value;
		ui32		ui32_value;
		float		float_value;
		TString*	string_value;
	};
};

/// Second one is here
/// One class holds string representation of appendix
/// Derived class constructors used for value to string conversion
/// String builder only appends data

class TStringFormatElementBase
{
public:
	byte*	Source;
	int		CharLength;
	int		ByteLength;

	char	PadChar;
	int		PadWidth; // 0 means no padding, positive numbers pads left side, negative numbers pads right side

	char	Tmp[24];
};

class TStringFormatElementInteger: public TStringFormatElementBase
{
public:
	inline TStringFormatElementInteger(int _value)
	{
		PadWidth = 0;
		ByteLength = TConvert::ToCharArrayInt(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}

	inline TStringFormatElementInteger(int _value, int _padwidth, char _padchar = ' ', TSign _sign = SGN_NEG)
	{
		PadWidth = _padwidth;
		PadChar = _padchar;
		ByteLength = TConvert::ToCharArrayInt(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}
};

class TStringFormatElementUnsignedInteger: public TStringFormatElementBase
{
public:
	inline TStringFormatElementUnsignedInteger(ui32 _value)
	{
		PadWidth = 0;
		ByteLength = TConvert::ToCharArrayUInt(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}

	inline TStringFormatElementUnsignedInteger(ui32 _value, int _padwidth, char _padchar = ' ')
	{
		PadWidth = _padwidth;
		PadChar = _padchar;
		ByteLength = TConvert::ToCharArrayInt(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}
};

class TStringFormatElementFloat: public TStringFormatElementBase
{
public:
	inline TStringFormatElementFloat(float _value)
	{
		PadWidth = 0;
		ByteLength = TConvert::ToCharArrayFloat(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}

	inline TStringFormatElementFloat(float _value, int _precision = 0, int _padwidth = 0, char _padchar = ' ')
	{
		PadWidth = _padwidth;
		PadChar = _padchar;
		ByteLength = TConvert::ToCharArrayFloat(_value,Tmp,24,_precision);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}
};

class TStringFormatElementHex: public TStringFormatElementBase
{
public:
	inline TStringFormatElementHex(ui32 _value)
	{
		PadWidth = 0;
		ByteLength = TConvert::ToCharArrayUIntHex(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}

	inline TStringFormatElementHex(ui32 _value, int _padwidth, char _padchar = ' ')
	{
		PadWidth = _padwidth;
		PadChar = _padchar;
		ByteLength = TConvert::ToCharArrayUIntHex(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}
};

class TStringFormatElementHexCapital: public TStringFormatElementBase
{
public:
	inline TStringFormatElementHexCapital(ui32 _value)
	{
		PadWidth = 0;
		ByteLength = TConvert::ToCharArrayUIntHexCapital(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}

	inline TStringFormatElementHexCapital(ui32 _value, int _padwidth, char _padchar = ' ')
	{
		PadWidth = _padwidth;
		PadChar = _padchar;
		ByteLength = TConvert::ToCharArrayUIntHexCapital(_value,Tmp,24);
		CharLength = ByteLength;
		Source = (byte*)Tmp;
	}
};

class TStringFormatElementString: public TStringFormatElementBase
{
public:
	inline TStringFormatElementString(const TString& _value)
	{
		PadWidth = 0;
		ByteLength = _value.ByteLength;
		CharLength = _value.Length;
		Source = _value.Data;
	}

	inline TStringFormatElementString(const TString& _value, int _padwidth, char _padchar = ' ')
	{
		PadWidth = _padwidth;
		PadChar = _padchar;
		ByteLength = _value.ByteLength;
		CharLength = _value.Length;
		Source = _value.Data;
	}
};

// Shorter naming of classes sf means "string format"
typedef TStringFormatElementBase				sfb;
//typedef const TStringFormatElementBase&			sfp; // parameter version

typedef TStringFormatElementInteger				sfi;
typedef TStringFormatElementUnsignedInteger		sfu;
typedef TStringFormatElementFloat				sff;
typedef TStringFormatElementHex					sfx;
typedef TStringFormatElementHexCapital			sfxc;
typedef TStringFormatElementString				sfs;

#endif