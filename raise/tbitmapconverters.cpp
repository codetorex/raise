#include "stdafx.h"
#include "tbitmap.h"
#include "tcompositeconverter.h"

class TBitmapConverterBGRtoRGB: public TCompositeConverter
{
public:
	TBitmapConverterBGRtoRGB()
	{
		SourceFormat = BitmapFormats->fBGR;
		DestinationFormat = BitmapFormats->fRGB;
	}

	void Convert(byte* src, byte* dst,int itemCount)
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

class TBitmapConverterARGBtoBGR: public TCompositeConverter
{
public:
	TBitmapConverterARGBtoBGR()
	{
		SourceFormat = BitmapFormats->fARGB;
		DestinationFormat = BitmapFormats->fBGR;
	}

	void Convert(byte* src, byte* dst,int pixelCount)
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

class TBitmapConverterRGBAtoBGR: public TCompositeConverter
{
public:
	TBitmapConverterRGBAtoBGR()
	{
		SourceFormat = BitmapFormats->fRGBA;
		DestinationFormat = BitmapFormats->fBGR;
	}

	void Convert(byte* src, byte* dst,int pixelCount)
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

class TBitmapConverterRGBAtoBGRA: public TCompositeConverter
{
public:
	TBitmapConverterRGBAtoBGRA()
	{
		SourceFormat = BitmapFormats->fRGBA;
		DestinationFormat = BitmapFormats->fBGRA;
	}

	void Convert(byte* src, byte* dst,int pixelCount)
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

class TBitmapConverterBGRtoBGRA: public TCompositeConverter
{
public:
	TBitmapConverterBGRtoBGRA()
	{
		SourceFormat = BitmapFormats->fBGR;
		DestinationFormat = BitmapFormats->fBGRA;
	}

	void Convert(byte* src, byte* dst,int pixelCount)
	{
		while(pixelCount--)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = 255;
			dst += 4;
			src += 3;
		}
	}
};

class TBitmapConverteRGBtoBGRA: public TCompositeConverter
{
public:
	TBitmapConverteRGBtoBGRA()
	{
		SourceFormat = BitmapFormats->fRGB;
		DestinationFormat = BitmapFormats->fBGRA;
	}

	void Convert(byte* src, byte* dst,int pixelCount)
	{
		while(pixelCount--)
		{
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = src[0];
			dst[3] = 255;
			dst += 4;
			src += 3;
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


	// 3 to 4 no swapping
	AddConverter(new TBitmapConverterBGRtoBGRA());

	//AddConverter("BGR", "BGRX", new TBitmapConverterBGRtoBGRA() );

	// 3 to 4 swapping

	AddConverter(new TBitmapConverteRGBtoBGRA());

	//AddConverter("RGB", "BGRX", new TBitmapConverteRGBtoBGRA() );

	AddConverter("BGR", "RGBA", new TBitmapConverteRGBtoBGRA() );

	//AddConverter("BGR", "RGBX", new TBitmapConverteRGBtoBGRA() );
}