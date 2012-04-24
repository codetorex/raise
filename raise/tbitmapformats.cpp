#include "stdafx.h"
#include "tbitmap.h"

TBitmapFormats* BitmapFormats = 0;

void TBitmapFormats::InitializeBitmapFormats()
{
	BitmapFormats = this;

	SetPrimitive(cRed,"Red (byte)","R8",tc_byte);
	SetPrimitive(cGreen,"Green (byte)","G8",tc_byte);
	SetPrimitive(cBlue,"Blue (byte)","B8",tc_byte);
	SetPrimitive(cAlpha,"Alpha (byte)","A8",tc_byte);


	fARGB = CreateFormat("ARGB","A8R8G8B8");
	fABGR = CreateFormat("ABGR","A8B8G8R8");
	fRGBA = CreateFormat("RGBA","R8G8B8A8");
	fBGRA = CreateFormat("BGRA","B8G8R8A8");
	fRGB = CreateFormat ("RGB" ,"R8G8B8");
	fBGR = CreateFormat ("BGR" ,"B8G8R8");
}

void TBitmapFormats::CreateDefaultFormats()
{
	BitmapFormats = new TBitmapFormats(&TCompositeFormatFactoryGeneric::Instance);
}