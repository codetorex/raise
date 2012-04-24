#include "stdafx.h"
#include "tgraphics.h"

void TBitmapGraphics::DrawLine( TPen& pen, int x1, int y1, int x2, int y2 )
{
	if (pen.Color.a != 255)
	{
		throw NotImplementedException();
	}

	TranslateCoord(x1,y1);
	TranslateCoord(x2,y2);

	int dy = y2 - y1;
	int dx = x2 - x1;
	int stepx, stepy;

	if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
	if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
	dy <<= 1;                                                  // dy is now 2*dy
	dx <<= 1;                                                  // dx is now 2*dx

	Bitmap->SetPixel(x1,y1, pen.Color);
	if (dx > dy) 
	{
		int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
		while (x1 != x2) 
		{
			if (fraction >= 0) 
			{
				y1 += stepy;
				fraction -= dx;                                // same as fraction -= 2*dx
			}
			x1 += stepx;
			fraction += dy;                                    // same as fraction -= 2*dy
			Bitmap->SetPixel(x1,y1,pen.Color);
		}
	} 
	else
	{
		int fraction = dx - (dy >> 1);
		while (y1 != y2) 
		{
			if (fraction >= 0) 
			{
				x1 += stepx;
				fraction -= dy;
			}
			y1 += stepy;
			fraction += dx;
			Bitmap->SetPixel(x1,y1,pen.Color);
		}
	}
}

void TBitmapGraphics::DrawImage( TBitmap& bmp, int dstX, int dstY, int srcX, int srcY, int width /*= -1*/, int height /*= -1*/ )
{
	if (width == -1)
	{
		width = bmp.Width;
	}

	if (height == -1)
	{
		height = bmp.Height;
	}

	if (Bitmap->BufferFormat != bmp.BufferFormat)
	{
		throw Exception("Incompatible formats");
	}

	int dx,dy = dstY;

	for (int y = srcY; y < height; y++)
	{
		dx = dstX;
		for (int x = srcX; x < width; x++)
		{
			byte* pix = bmp.GetPixel(x,y);
			Bitmap->SetPixel(dx,dy,pix);
			dx++;
		}
		dy++;
	}
}

void TBitmapGraphics::FlipHorizontal()
{
	// This function should be fixed like flipvertical. which is about only exchange halfs
	// not continue to exchange after passed half part
	throw Exception("Call of incomplete function");

	int x1 = Bitmap->Width;
	int x2 = 0;

	int rowwidth = Bitmap->Width * Bitmap->BufferFormat->BytesPerItem; // dont worry this is correct
	int pixelSize = Bitmap->BufferFormat->BytesPerItem;

	byte tmp[64];

	while(x1--)
	{
		byte* column1 = GetPixel(x1,0);
		byte* column2 = GetPixel(x2,0);

		int i = Bitmap->Height;
		while(i--)
		{
			MemoryDriver::ShortCopy(tmp,column2,pixelSize);
			MemoryDriver::ShortCopy(column2,column1,pixelSize);
			MemoryDriver::ShortCopy(column1,tmp,pixelSize);

			column1 += rowwidth; // advance memory pointer
			column2 += rowwidth; // advance memory pointer
		}
	}
}

void TBitmapGraphics::FlipVertical()
{
	// algorithm is exchanges data between vertical half of image
	// so if image's height is even then all rows will be exchanged
	// if its odd then only 1 row will be left to be not exchanged

	int rows = Bitmap->Height / 2; // div to 2 makes this. so if first bit is set, which means its odd it will be gone
	int y1 = Bitmap->Height - 1; 
	int y2 = 0;
	int rowwidth = Bitmap->Width * Bitmap->BufferFormat->BytesPerItem;

	while(rows--)
	{
		byte* row1 = GetPixel(0,y1);
		byte* row2 = GetPixel(0,y2);

		MemoryDriver::Exchange(row1,row2,rowwidth);
		y2++;
		y1--;
	}
}
TGraphics* TGraphics::FromBitmap( TBitmap* bitmap )
{
	if (bitmap->BufferFormat->BytesPerItem != 4)
	{
		throw Exception("Buffer format is not compatible with graphics");
	}

	return new TBitmapGraphics(bitmap);

	/*TBinary wbinary(bitmap->Width);
	ui32 bytesPerPixel = bitmap->BufferFormat->BytesPerItem;
	bool powerOf2 = wbinary.is2n();
	if (powerOf2 && bytesPerPixel == 4)
	{
		ui32 log2width = wbinary.log2k();
		return new TBitmap32Bpp2nGraphics(bitmap,log2width);
	}

	if (powerOf2)
	{

	}

	// not power of 2 and not optimizable bitmap

	throw NotImplementedException();
	return 0;*/
}

/*
void TBitmap::ColorKey( const TColor24& clr,byte alpha )
{
	Convert(BitmapFormats->fARGB);

	TColor32* pix = (TColor32*)Data;

	int i = PixelCount;
	while(i--)
	{
		if (*(pix++) == clr)
		{
			pix->a = alpha;
		}
	}

}

void TBitmap::Copy( TBitmap* src,int dstX,int dstY, int srcX, int srcY, int _width,int _height  )
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
			byte* pix = src->GetPixel(x,y);
			SetPixel(dx,dy,pix);
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
		byte* rowstart = GetPixel(_x,y);
		int rpix = _width;
		while(rpix--)
		{
			MemoryDriver::ShortCopy(rowstart,pClr,pixelWidth);
			rowstart += pixelWidth;
		}
	}
}*/