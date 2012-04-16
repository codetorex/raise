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
		TBitmap bmp(128,128);
	}

} TGraphicsBitmapCheck;

// TEST writing PNG and reading from PNG too

#endif