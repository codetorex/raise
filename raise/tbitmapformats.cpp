#include "stdafx.h"
#include "tbitmap.h"

TFormatElement TBitmapChannels::cRed("Red (byte)","R8",tc_byte);
TFormatElement TBitmapChannels::cGreen("Green (byte)","G8",tc_byte);
TFormatElement TBitmapChannels::cBlue("Blue (byte)","B8",tc_byte);
TFormatElement TBitmapChannels::cAlpha("Alpha (byte)","A8",tc_byte);

TFormatElement* teElements[] = 
{ 
	&TBitmapChannels::cRed, 
	&TBitmapChannels::cGreen, 
	&TBitmapChannels::cBlue, 
	&TBitmapChannels::cAlpha,
	NULL,
};

TBufferFormat TBitmapFormats::fARGB("ARGB",teElements,"A8R8G8B8");
TBufferFormat TBitmapFormats::fABGR("ABGR",teElements,"A8B8G8R8");
TBufferFormat TBitmapFormats::fRGBA("RGBA",teElements,"R8G8B8A8");
TBufferFormat TBitmapFormats::fBGRA("BGRA",teElements,"B8G8R8A8");
TBufferFormat TBitmapFormats::fRGB("RGB",teElements,"R8G8B8");
TBufferFormat TBitmapFormats::fBGR("BGR",teElements,"B8G8R8");
