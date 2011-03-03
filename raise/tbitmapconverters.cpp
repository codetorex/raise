#include "stdafx.h"
#include "tbitmap.h"

class TBitmapConverterBGRtoRGB: public TBufferFormatConverter
{
public:
	TBitmapConverterBGRtoRGB()
	{
		SourceFormat = TBitmapFormats::fBGR;
		DestinationFormat = TBitmapFormats::fRGB;
	}

	void Convert(TFlexibleBuffer* srcBuffer)
	{
		TBitmap* bmp = (TBitmap*)srcBuffer;
		byte* src = srcBuffer->Buffer;
		int curpixel = bmp->pixels;
		byte tmp;
		while(curpixel--)
		{
			tmp = *src;
			src[0] = src[2];
			src[2] = tmp;
			src += 3;
		}
		srcBuffer->BufferFormat = DestinationFormat;
	}
};

void TBitmapFormats::CreateDefaultConverters()
{
	TBitmapConverterBGRtoRGB* BGRtoRGB = new TBitmapConverterBGRtoRGB();
	TBitmapFormats::fBGR->Converters.Add(BGRtoRGB);
}