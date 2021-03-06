#include "stdafx.h"
#include "tgraphics.h"

void TBitmapGraphics::DrawLine( TPen& pen, int x1, int y1, int x2, int y2 )
{
	if (pen.Color.a != 255)
	{
		throw NotImplementedException(__FILE__,__LINE__);
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

	Bitmap->SetPixel(x1,y1, pen.Color.bclr);
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
			Bitmap->SetPixel(x1,y1,pen.Color.bclr);
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
			Bitmap->SetPixel(x1,y1,pen.Color.bclr);
		}
	}
}

#include "tcompositeconverter.h"


void TBitmapGraphics::DrawImage( TBitmap& srcBmp, int dstX, int dstY, int srcX, int srcY, int width , int height )
{
	if (dstX + width > Bitmap->Width)
	{
		width = srcBmp.Width - dstX;
	}

	if (dstY + height > Bitmap->Height)
	{
		height = srcBmp.Height - dstY;
	}

	TBitmap& dstBmp = *Bitmap;


	if (dstBmp.BufferFormat != srcBmp.BufferFormat)
	{
		TCompositeConverter* Converter = srcBmp.BufferFormat->GetConverter(dstBmp.BufferFormat);
		if (Converter == 0)
		{
			throw Exception("Incompatible formats and no converter");
		}

		for (int l = 0; l < height; l++)
		{
			byte* srcLine = srcBmp.GetPixel(srcX,srcY + l);
			byte* dstLine = dstBmp.GetPixel(dstX,dstY + l);

			if (BlendMode == &TBlendModes::Copy)
			{
				Converter->Convert(srcLine,dstLine,width);
			}
			else
			{
				int srcItemLength = srcBmp.BufferFormat->BytesPerItem;
				for (int x = 0 ; x < width ; x++)
				{
					TColor32 srcColor;
					Converter->Convert(srcLine,srcColor.bclr,1); // convert 1 pixel
					BlendMode->Blend(srcColor, *(TColor32*)dstLine);

					srcLine += srcItemLength;
					dstLine += 4;
				}
			}
		}
	}
	else
	{
		ui32 lineLength = dstBmp.BufferFormat->BytesPerItem * width;
		for (int l = 0; l < height; l++)
		{
			byte* srcLine = srcBmp.GetPixel(srcX,srcY + l);
			byte* dstLine = dstBmp.GetPixel(dstX,dstY + l);
			if (BlendMode == &TBlendModes::Copy)
			{
				MemoryDriver::Copy(dstLine,srcLine, lineLength);
			}
			else
			{
				BlendMode->BlendArray((TColor32*)srcLine,(TColor32*)dstLine,lineLength);
			}
		}
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
		byte* column1 = Bitmap->GetPixel(x1,0);
		byte* column2 = Bitmap->GetPixel(x2,0);

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
		byte* row1 = Bitmap->GetPixel(0,y1);
		byte* row2 = Bitmap->GetPixel(0,y2);

		MemoryDriver::Exchange(row1,row2,rowwidth);
		y2++;
		y1--;
	}
}

#include "mangle.h"

void TBitmapGraphics::DrawEllipse( TPen& pen, int x, int y, int width, int height )
{
	/*int lastAngX = width / 2;
	int lastAngY = 0;
	float lastAngle = 15.0f;

	float xwidth = width / 2;
	float yheight = height / 2;

	for (int i=0;i<6;i++)
	{
		MDegree<float> angl(lastAngle);

		int angX = xwidth * angl.Sine();
		int angY = yheight * angl.Cosine();

		DrawLine(pen, x + lastAngX , y + lastAngY,)

	}*/
}

void TBitmapGraphics::Clear( const TColor32& color )
{
	MemoryDriver::Repeat4Byte(Bitmap->Data,color.bclr,Bitmap->PixelCount);
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

	throw NotImplementedException(__FILE__,__LINE__);
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