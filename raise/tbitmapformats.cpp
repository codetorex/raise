#include "stdafx.h"
#include "tbitmap.h"

TCompositionPrimitive* TBitmapChannels::cRed = 0;
TCompositionPrimitive* TBitmapChannels::cGreen = 0;
TCompositionPrimitive* TBitmapChannels::cBlue = 0;
TCompositionPrimitive* TBitmapChannels::cAlpha = 0;

TArray<TCompositionPrimitive*>* TBitmapChannels::BitmapChannelRegistry = 0;

TBufferFormat* TBitmapFormats::fARGB = 0;
TBufferFormat* TBitmapFormats::fABGR = 0;
TBufferFormat* TBitmapFormats::fRGBA = 0;
TBufferFormat* TBitmapFormats::fBGRA = 0;
TBufferFormat* TBitmapFormats::fRGB = 0;
TBufferFormat* TBitmapFormats::fBGR = 0;

TArray<TBufferFormat*>* TBitmapFormats::BitmapFormatRegistry = 0;

void TBitmapChannels::CreateDefaultChannels()
{
	cRed = new TCompositionPrimitive("Red (byte)","R8",tc_byte);
	cGreen = new TCompositionPrimitive("Green (byte)","G8",tc_byte);
	cBlue = new TCompositionPrimitive("Blue (byte)","B8",tc_byte);
	cAlpha = new TCompositionPrimitive("Alpha (byte)","A8",tc_byte);

	BitmapChannelRegistry = new TArray<TCompositionPrimitive*> ();
	BitmapChannelRegistry->Add(cRed);
	BitmapChannelRegistry->Add(cGreen);
	BitmapChannelRegistry->Add(cBlue);
	BitmapChannelRegistry->Add(cAlpha);
}

void TBitmapFormats::CreateDefaultFormats()
{
	fARGB = new TBufferFormat ("ARGB",TBitmapChannels::BitmapChannelRegistry,"A8R8G8B8");
	fABGR = new TBufferFormat ("ABGR",TBitmapChannels::BitmapChannelRegistry,"A8B8G8R8");
	fRGBA = new TBufferFormat ("RGBA",TBitmapChannels::BitmapChannelRegistry,"R8G8B8A8");
	fBGRA = new TBufferFormat ("BGRA",TBitmapChannels::BitmapChannelRegistry,"B8G8R8A8");
	fRGB = new TBufferFormat ("RGB",TBitmapChannels::BitmapChannelRegistry,"R8G8B8");
	fBGR = new TBufferFormat ("BGR",TBitmapChannels::BitmapChannelRegistry,"B8G8R8");

	BitmapFormatRegistry = new TArray<TBufferFormat*> ();
	BitmapFormatRegistry->Add(fARGB);
	BitmapFormatRegistry->Add(fABGR);
	BitmapFormatRegistry->Add(fRGBA);
	BitmapFormatRegistry->Add(fBGRA);
	BitmapFormatRegistry->Add(fRGB);
	BitmapFormatRegistry->Add(fBGR);
}

