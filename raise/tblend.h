#ifndef TBLEND_H
#define TBLEND_H

#include "tcolor.h"

enum TBlendModeType
{
	BO_COPY, // no blending
	BO_NORMAL,
	/*BO_DARKEN,
	BO_MULTIPLY,
	BO_COLOR_BURN,
	BO_LINEAR_BURN,
	BO_LIGHTEN,
	BO_SCREEN,
	BO_COLOR_DODGE,
	BO_LINEAR_DODGE,
	BO_OVERLAY,
	BO_SOFTLIGHT,
	BO_HARDLIGHT,
	BO_LINEAR_LIGHT,
	BO_PIN_LIGHT,
	BO_DIFFERENCE,
	BO_EXCLUSION,
	BO_ADDITIVE,
	BO_REFLECT,
	BO_GLOW,
	BO_HEAT,
	BO_FREEZE,
	BO_AVERAGE,
	BO_BRIGHTLIGHT,
	BO_NEGATION,*/
};

class TBlendMode
{
public:
	/**
	 * Blends source color with destination color and writes to destination.
	 * Basically dst = src (*) dst
	 */
	virtual void Blend(const TColor32& src, TColor32& dst) = 0;

	inline void BlendArray(const TColor32* src, TColor32* dst, ui32 count)
	{
		while(count--)
		{
			Blend(*(src++),*(dst++));
		}
	}
};

class TBlendCopy: public TBlendMode
{
public:
	void Blend(const TColor32& src, TColor32& dst)
	{
		dst = src;
	}
};

/**
 * Algorithm stolen from Wikipedia article :D 
 * http://en.wikipedia.org/wiki/Alpha_compositing
 */
class TBlendWiki: public TBlendMode
{
public:
	TColorFloat BlendFloat(const TColorFloat& src, const TColorFloat& dst)
	{
		TColorFloat result(0,0,0,0);

		result.A = src.A + dst.A * ( 1.0f - src.A);
		if (result.A < (0.9f / 255.0f))
		{
			return result;
		}

		if (result.A >= (254.1f / 255.0f))
		{
			result.R = src.R * src.A + dst.R * ( 1.0f - src.A);
			result.G = src.G * src.A + dst.G * ( 1.0f - src.A);
			result.B = src.B * src.A + dst.B * ( 1.0f - src.A);
			return result;
		}

		result.R = (src.R * src.A + dst.R * dst.A * ( 1.0f - src.A)) / result.A;
		result.G = (src.G * src.A + dst.G * dst.A * ( 1.0f - src.A)) / result.A;
		result.B = (src.B * src.A + dst.B * dst.A * ( 1.0f - src.A)) / result.A;
		return result;
	}

	// TODO: implement this to work with byte one instead of float
	inline void Blend(const TColor32& src, TColor32& dst)
	{
		TColorFloat result = BlendFloat(src,dst);
		dst = result.ConvertToByteColor();
	}
};

// 

class TBlendOpNormal
{
public:
	inline void BlendChannel(int& dst, int& src, int mix)
	{
		dst = ((((dst) << 8) + ((src) - (dst)) * (mix)) >> 8);
	}
};

template <class T>
class TBlendGeneric: public TBlendMode
{
	inline void Blend(const TColor32& src, TColor32& dst)
	{
		if (src.a == 0)
		{
			return;
		}

		int mix_alpha;

		int dst_red = dst.r;
		int dst_green = dst.g;
		int dst_blue = dst.b;
		int dst_alpha = dst.a;

		int src_red = src.r;
		int src_green = src.g;
		int src_blue = src.b;
		int src_alpha = src.a;

		switch(dst.a)
		{
		case 0:
			dst = src;
			return;

		case 255:
			mix_alpha = src_alpha;
			break;

		default:
			mix_alpha = (src_alpha << 8) / (src_alpha + ((256 - src_alpha) * dst_alpha >> 8));
			dst_alpha = dst_alpha + ((256 - dst_alpha) * src_alpha >> 8);
		}

		T func;

		func.BlendChannel(dst_red,src_red,mix_alpha);
		func.BlendChannel(dst_green,src_green,mix_alpha);
		func.BlendChannel(dst_blue,src_blue,mix_alpha);

		dst = TColor32(dst_red,dst_green,dst_blue,dst_alpha);
	}
};

class TBlendModes
{
public:
	static TBlendCopy Copy;
	static TBlendWiki HighPrecision;
	static TBlendGeneric< TBlendOpNormal > Normal;
};



#endif