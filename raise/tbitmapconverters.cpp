#include "stdafx.h"
#include "tbitmap.h"
#include "tcompositegenericconverter.h"

class TBitmapConverterBGRtoRGB: public TCompositeGenericConverter
{
public:
	TBitmapConverterBGRtoRGB()
	{
		SourceFormat = BitmapFormats->fBGR;
		DestinationFormat = BitmapFormats->fRGB;
	}

	void DoConversion(byte* src, byte* dst,int itemCount)
	{
		while(itemCount--)
		{
			byte tmp = src[0];
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = tmp;
			dst += 3;
			src += 3;
		}
	}
};

class TBitmapConverterARGBtoBGR: public TCompositeGenericConverter
{
public:
	TBitmapConverterARGBtoBGR()
	{
		SourceFormat = BitmapFormats->fARGB;
		DestinationFormat = BitmapFormats->fBGR;
	}

	void DoConversion(byte* src, byte* dst,int pixelCount)
	{
		while(pixelCount--)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			src += 4;
			dst += 3;
		}
	}
};

void TBitmapFormats::CreateDefaultConverters()
{
	TBitmapConverterBGRtoRGB* BGRtoRGB = new TBitmapConverterBGRtoRGB();
	fBGR->Converters.Add(BGRtoRGB);

	// use same converter for BGR to RGB conversion
	TBitmapConverterBGRtoRGB* RGBtoBGR = new TBitmapConverterBGRtoRGB(); 
	RGBtoBGR->SourceFormat = TBitmapFormats::fRGB;
	RGBtoBGR->SourceFormat = TBitmapFormats::fBGR;
	fRGB->Converters.Add(RGBtoBGR);

	TBitmapConverterARGBtoBGR* ARGBtoBGR = new TBitmapConverterARGBtoBGR();
	fARGB->Converters.Add(ARGBtoBGR);
}