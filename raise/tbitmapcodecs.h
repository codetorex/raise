#ifndef TBITMAPCODECS_H
#define TBITMAPCODECS_H

#include "tbitmap.h"

#include "tbitmapbmp.h"
#include "tbitmaptga.h"
#include "tbitmappng.h"

class TBitmapCodecs
{
public:
	static TBitmapPNG Png;
	static TBitmapBMP Bmp;
	static TBitmapTGA Tga;

	//static TBitmapJPG Jpeg;
	//static TBitmapGIF Gif;
	//static TBitmapTIF Tiff;
};

#endif