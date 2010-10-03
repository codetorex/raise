#include "stdafx.h"
#include "tbitmap.h"
#include "tbinary.h"

TBitmap::TBitmap()
{
	data = 0;
	length = 0;
	width = 0;
	height = 0;
	pixels = 0;
	bits = 0;
	bytes = 0;
	format = 0;
	log2width = 0;
}

TBitmap::TBitmap(int _width,int _height, int _format)
{
//	create(_width,_height,_format);
}

void TBitmap::create(int _width,int _height,int _bpp, int _format)
{
	bytes = DIV8(_bpp);
	pixels = _width * _height;
	length = pixels * bytes;
	data = new byte[length];
	width = _width;
	height = _height;
	bits = _bpp;
	format = _format;

	if (bytes == 4)
		format |= O4BYTE;

	TBinary wbinary(width);
	if (wbinary.is2n())
	{
		format |= O2NWIDTH;
		log2width = wbinary.log2_lookup_ik();
	}
}

void TBitmap::release()
{
	if (data)
	{
		delete [] data;
	}

}

void TBitmap::loadbmp(Stream* bmpstream,bool closestream /* = true */)
{
	
}

void TBitmap::drawline(int x,int y,int x2,int y2,byte* pClr)
{	
	int dy = y2 - y;
	int dx = x2 - x;
	int stepx, stepy;

	if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
	if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
	dy <<= 1;                                                  // dy is now 2*dy
	dx <<= 1;                                                  // dx is now 2*dx

	setpixel(x,y,pClr);
	if (dx > dy) 
	{
		int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
		while (x != x2) 
		{
			if (fraction >= 0) 
			{
				y += stepy;
				fraction -= dx;                                // same as fraction -= 2*dx
			}
			x += stepx;
			fraction += dy;                                    // same as fraction -= 2*dy
			setpixel(x,y,pClr);
		}
	} 
	else
	{
		int fraction = dx - (dy >> 1);
		while (y != y2) 
		{
			if (fraction >= 0) 
			{
				x += stepx;
				fraction -= dy;
			}
			y += stepy;
			fraction += dx;
			setpixel(x,y,pClr);
		}
	}
}