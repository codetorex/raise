#include "stdafx.h"
#include "tbitmap.h"




/*static int fieldstart[] =
{
	0,
	8,
	12,
	16,
	20,
};

static int fieldlength[] =
{
	8, // channels included
	4, // channel data types
	4, // channel ordering
	4, // optimization flags
};

#define BFCHANNEL		0
#define BFCHNTYPE		1
#define BFCHNORDR		2
#define BFOPFLAGS		3*/



TBitmap::TBitmap(): TFlexibleBuffer()
{
	Width = 0;
	Height = 0;
	pixels = 0;
	log2width = 0;
}

TBitmap::TBitmap(int _width,int _height, TBufferFormat* _format)
{
	create(_width,_height,_format);
}

void TBitmap::create(int _width,int _height, TBufferFormat* _format)
{

	BufferFormat = _format;
	flags = 0;

	pixels = _width * _height;

	Allocate(pixels);

	Width = _width;
	Height = _height;

	if (BufferFormat->BytesPerItem == 4)
		flags |= O32BITALIGN;

	TBinary wbinary(Width);
	if (wbinary.is2n())
	{
		flags |= O2NWIDTH;
		log2width = wbinary.log2k();
	}
}

void TBitmap::release()
{
	Free();
	Width = Height = pixels = log2width = 0;
}

void TBitmap::DrawLine(int x,int y,int x2,int y2,byte* pClr)
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

void TBitmap::FlipHorizontal()
{
	// This function should be fixed like flipvertical. which is about only exchange halfs
	// not continue to exchange after passed half part
	throw Exception("Call of incomplete function");

	int x1 = Width;
	int x2 = 0;

	int rowwidth = Width * BufferFormat->BytesPerItem; // dont worry this is correct
	int pixelSize = BufferFormat->BytesPerItem;

	byte tmp[64];

	while(x1--)
	{
		byte* column1 = getpixel(x1,0);
		byte* column2 = getpixel(x2,0);

		int i = Height;
		while(i--)
		{
			PixelCopy(tmp,column2,pixelSize);
			PixelCopy(column2,column1,pixelSize);
			PixelCopy(column1,tmp,pixelSize);

			column1 += rowwidth; // advance memory pointer
			column2 += rowwidth; // advance memory pointer
		}
	}
}

void TBitmap::FlipVertical()
{
	// algorithm is exchanges data between vertical half of image
	// so if image's height is even then all rows will be exchanged
	// if its odd then only 1 row will be left to be not exchanged

	int rows = DIV2(Height); // div to 2 makes this. so if first bit is set, which means its odd it will be gone
	int y1 = Height-1; 
	int y2 = 0;
	int rowwidth = Width * BufferFormat->BytesPerItem;

	while(rows--)
	{
		byte* row1 = getpixel(0,y1);
		byte* row2 = getpixel(0,y2);

		MemoryDriver::Exchange(row1,row2,rowwidth);
		y2++;
		y1--;
	}
}

void TBitmap::ColorKey( const TColor24& clr,byte alpha /*= 0*/ )
{
	Convert(TBitmapFormats::fARGB);

	TColor32* pix = (TColor32*)Buffer;

	int i = pixels;
	while(i--)
	{
		if (*(pix++) == clr)
		{
			pix->a = alpha;
		}
	}

}

void TBitmap::Copy( TBitmap* src,int dstX,int dstY, int srcX /*= 0*/, int srcY /*= 0*/, int _width /*= -1*/,int _height /*= -1*/ )
{
	if (_width == -1)
	{
		_width = src->Width;
	}

	if (_height == -1)
	{
		_height = src->Height;
	}

	if (BufferFormat != src->BufferFormat)
	{
		throw Exception("Incompatible formats");
	}

	int dx,dy = dstY;

	for (int y = srcY; y < _height; y++)
	{
		dx = dstX;
		for (int x = srcX; x < _width; x++)
		{
			byte* pix = src->getpixel(x,y);
			setpixel(dx,dy,pix);
			dx++;
		}
		dy++;
	}
}

void TBitmap::DrawRectangle( int _x,int _y,int _width,int _height,byte* pClr )
{
	if (_x > Width) return;
	if (_x + _width > Width) _width = Width - _x;
	if (_y + _height > Height) _height = Height - _y;

	int ibottom = _y + _height;
	int pixelWidth = BufferFormat->BytesPerItem;
	for (int y = _y ;y<ibottom;y++)
	{
		byte* rowstart = getpixel(_x,y);
		int rpix = _width;
		while(rpix--)
		{
			PixelCopy(rowstart,pClr,pixelWidth);
			rowstart += pixelWidth;
		}
	}
}