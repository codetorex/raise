#ifndef TBITMAPFORMATS_H
#define TBITMAPFORMATS_H

#include "tcompositebuffer.h"

// Defines default Bitmap Channels
class TBitmapChannels
{
public:
	void CreateDefaultChannels();

	virtual void Initialize()
	{
		CreateDefaultChannels();
	}

	static TCompositionPrimitive* cRed;
	static TCompositionPrimitive* cGreen;
	static TCompositionPrimitive* cBlue;
	static TCompositionPrimitive* cAlpha;

	static TArray<TCompositionPrimitive*>* BitmapChannelRegistry;
};

// Defines default Bitmap Formats
class TBitmapFormats
{
public:
	static TBufferFormat* fARGB;
	static TBufferFormat* fABGR;
	static TBufferFormat* fRGBA;
	static TBufferFormat* fBGRA;
	static TBufferFormat* fRGB;
	static TBufferFormat* fBGR;

	static TArray<TBufferFormat*>* BitmapFormatRegistry;

	void CreateDefaultFormats();
	void CreateDefaultConverters();

	virtual void Initialize()
	{
		CreateDefaultFormats();
		CreateDefaultConverters();
	}
};

#include "tbitmapbmp.h"
#include "tbitmaptga.h"
#include "tbitmappng.h"

#endif