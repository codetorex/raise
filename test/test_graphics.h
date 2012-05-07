#ifndef TEST_GRAPHICS_H
#define TEST_GRAPHICS_H

#include "tgraphics.h"

class TestTGraphicsBitmap: public TTestCheck
{
public:

	TestTGraphicsBitmap(): TTestCheck("TGraphicsBitmap check") 
	{

	}

	void Test()
	{
		TBrush testBrush(TColors::Aqua);

		TBitmap bmp(128,128);

		TGraphics* gfx = TGraphics::FromBitmap(&bmp);

		gfx->FillRectangle(testBrush,0,0,16,16);

		TColor32 p1 = gfx->GetPixel(0,0);
		TColor32 p2 = gfx->GetPixel(15,15);
		TColor32 p3 = gfx->GetPixel(15,0);
		TColor32 p4 = gfx->GetPixel(0,15);

		AddResult(p1 == TColors::Aqua, "Fill rectangle top left corner");
		AddResult(p2 == TColors::Aqua, "Fill rectangle bottom right corner");
		AddResult(p3 == TColors::Aqua, "Fill rectangle top right corner");
		AddResult(p4 == TColors::Aqua, "Fill rectangle bottom left corner");
		
		p1 = gfx->GetPixel(16,16);
		p2 = gfx->GetPixel(16,0);
		p3 = gfx->GetPixel(0,16);

		AddResult(p1 != TColors::Aqua, "Fill rectangle outside test bottom right corner");
		AddResult(p2 != TColors::Aqua, "Fill rectangle outside test top right corner");
		AddResult(p3 != TColors::Aqua, "Fill rectangle outside test bottom left");

		bmp.Save("test.png");
		bmp.Save("test.bmp");
		

	}

} TGraphicsBitmapCheck;

// TEST writing PNG and reading from PNG too

#endif