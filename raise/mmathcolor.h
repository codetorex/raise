#ifndef MMATHCOLOR_H
#define MMATHCOLOR_H

#include "tcolor.h"

class MathColor
{
public:
	/*inline static TColor32 BlendNormal(TColor32& pc, TColor32& bg) 
	{
		int ax = (65025 - (255 - pc.a) * (255 - bg.a)) / 255;
		int rx = ((pc.r * pc.a) / ax) + ((bg.r * bg.a * (255 - pc.a)) / ax);
		int gx = ((pc.g * pc.a) / ax) + ((bg.g * bg.a * (255 - pc.a)) / ax);
		int bx = ((pc.b * pc.a) / ax) + ((bg.b * bg.a * (255 - pc.a)) / ax);
		return TColor32(rx,gx,bx,ax);
	}

	inline static TColorFloat BlendNormal(TColorFloat& pc, TColorFloat& bg)
	{
		float ax = 1.0f - (1.0f - pc.A) * (1.0f - bg.A);
		float rx = pc.R * pc.A / ax + bg.R * bg.A * (1 - pc.A) / ax;
		float gx = pc.G * pc.A / ax + bg.G * bg.A * (1 - pc.A) / ax;
		float bx = pc.B * pc.A / ax + bg.B * bg.A * (1 - pc.A) / ax;
		return TColorFloat(rx,gx,bx,ax);
	}

	inline static TColorFloat BlendAlpha(TColorFloat& src, TColorFloat& dst)
	{
		float outa = src.A + dst.A * ( 1.0f - src.A);
		float outr = (src.R * src.A + dst.R * dst.A * (1.0f - src.A)) /  outa;
		float outg = (src.G * src.A + dst.G * dst.A * (1.0f - src.A)) /  outa;
		float outb = (src.B * src.A + dst.B * dst.A * (1.0f - src.A)) /  outa;

	}*/

	/*inline static TColor32 Blend(TColor32& src, TColor32& dst)
	{
		TColor32 result;
		float a1 = src.A / 255.0;
		float a2 = dst.A / 255.0;

		result.R = (int) (a1 * src.r + a2 * (1 - a1) * dst.r);
		result.G = (int) (a1 * src.g + a2 * (1 - a1) * dst.g);
		result.B = (int) (a1 * src.b + a2 * (1 - a1) * dst.b);
		result.A = (int) (255 * (a1 + a2 * (1 - a1)));
		return result;
	}*/

	// TODO: move this to TColors as static function and delete this file?
	inline static TString ChannelOrder(byte* data, int length)
	{
		TString result;

		for (int i=0;i<length;i++)
		{
			byte ch = data[i];
			switch(ch)
			{
			case 64:
				result += 'R';
				break;

			case 128:
				result += 'G';
				break;

			case 178:
				result += 'B';
				break;

			case 255:
				result += 'A';
				break;

			default:
				result += '?';
				break;
			}
		}

		return result;
	}
};


#endif