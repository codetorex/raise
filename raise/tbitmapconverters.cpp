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
		// src 0 is alpha, 1 is r , 2 is g, 3 is b
		// dst 0 is b, dst 1 is g , dst 2 is r
		while(pixelCount--)
		{

			dst[0] = src[3];
			dst[1] = src[2];
			dst[2] = src[1];
			src += 4;
			dst += 3;
		}
	}
};

class TBitmapConverterRGBAtoBGR: public TCompositeGenericConverter
{
public:
	TBitmapConverterRGBAtoBGR()
	{
		SourceFormat = BitmapFormats->fRGBA;
		DestinationFormat = BitmapFormats->fBGR;
	}

	void DoConversion(byte* src, byte* dst,int pixelCount)
	{
		while(pixelCount--)
		{
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = src[0];
			src += 4;
			dst += 3;
		}
	}
};

class TBitmapConverterRGBAtoBGRA: public TCompositeGenericConverter
{
public:
	TBitmapConverterRGBAtoBGRA()
	{
		SourceFormat = BitmapFormats->fRGBA;
		DestinationFormat = BitmapFormats->fBGRA;
	}

	void DoConversion(byte* src, byte* dst,int pixelCount)
	{
		while(pixelCount--)
		{
			byte tmp = src[0]; // so this will make it support inplace exchange
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = tmp;
			dst[3] = src[3];
			dst += 4;
			src += 4;
		}
	}
};

void TBitmapFormats::CreateDefaultConverters()
{
	// 3 to 3 swapping
	AddConverter(new TBitmapConverterBGRtoRGB());

	AddConverter("RGB","BGR",new TBitmapConverterBGRtoRGB());

	// 4 to 3 swapping
	AddConverter( new TBitmapConverterARGBtoBGR() );

	AddConverter( new TBitmapConverterRGBAtoBGR() );

	// 4 to 4 swapping
	AddConverter( new TBitmapConverterRGBAtoBGRA() );

	AddConverter("BGRA","RGBA",new TBitmapConverterRGBAtoBGRA() );
}