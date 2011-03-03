#include "stdafx.h"
#include "tbitmap.h"

#ifdef LINUX 

struct BITMAPFILEHEADER 
{
	word bfType;
	dword bfSize;
	word bfReserved1;
	word bfReserved2;
	dword bfOffBits;
} __attribute((packed));

struct BITMAPINFOHEADER
{
	dword biSize;
	int biWidth;
	int biHeight;
	word biPlanes;
	word biBitCount;
	dword biCompression;
	dword biSizeImage;
	dword biXPelsPerMeter;
	dword biYPelsPerMeter;
	dword biClrUsed;
	dword biClrImportant;
}  __attribute((packed));

#endif



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
	width = 0;
	height = 0;
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

	Allocate(pixels * BufferFormat->BytesPerItem);

	width = _width;
	height = _height;

	if (BufferFormat->BytesPerItem == 4)
		flags |= O32BITALIGN;

	TBinary wbinary(width);
	if (wbinary.is2n())
	{
		flags |= O2NWIDTH;
		log2width = wbinary.log2k();
	}
}

void TBitmap::release()
{
	Free();
	width = height = pixels = log2width = 0;
}

#include "tfilestream.h"

void TBitmap::loadbmp(Stream* bmpstream, bool convertRGB, bool closestream /* = true */)
{
	int y;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	
	bmpstream->Read(&fileHeader,sizeof(BITMAPFILEHEADER),1);
	bmpstream->Read(&infoHeader,sizeof(BITMAPINFOHEADER),1);
	

	if (infoHeader.biCompression != 0 || (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32))
	{
		if(closestream) bmpstream->Close();
		throw Exception("Unsupported BMP format");
		return;
	}

	bool bottomUp = true;
	if (infoHeader.biHeight < 0)
	{
		bottomUp = false;
		infoHeader.biHeight = abs(infoHeader.biHeight);
	}

	release();
	
	BufferFormat = TBitmapFormats::fBGR;
	if(infoHeader.biBitCount == 32)
	{
		BufferFormat = TBitmapFormats::fRGBA;
	}

	create(infoHeader.biWidth,infoHeader.biHeight,BufferFormat);

	int rowWidth = infoHeader.biWidth * BufferFormat->BytesPerItem; // row width for file, has padding
	int realRowWidth = rowWidth; // row width for bitmap, doesnt has padding
	rowWidth += MOD4(rowWidth);
	byte* row = new byte[rowWidth];

	
	if (bottomUp)
	{
		y = height;
		while (y--)
		{
			byte* rowstart = getpixel(0,y);
			bmpstream->Read(row,1,rowWidth);
			memcpy(rowstart,row,realRowWidth);
		}
	}
	else
	{
		for (y = 0;y<height;y++)
		{
			byte* rowstart = getpixel(0,y);
			bmpstream->Read(row,1,rowWidth);
			memcpy(rowstart,row,realRowWidth);
		}
	}

	TFileStream* fs = new TFileStream("c:\\book.bmp",fm_Write);
	savebmp(fs,true);
	delete fs;

	if (convertRGB && BufferFormat == TBitmapFormats::fBGR)
	{
		Convert(TBitmapFormats::fRGB);
	}

	if (closestream)
	{
		bmpstream->Close();
	}
}


void TBitmap::savebmp( Stream* bmpstream,bool closestream /*= true*/ )
{
	int y;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = pixels * 3;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;
	infoHeader.biWidth = width;
	infoHeader.biHeight = height;

	fileHeader.bfSize = sizeof(BITMAPFILEHEADER);
	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmpstream->Write(&fileHeader,1,sizeof(BITMAPFILEHEADER));
	bmpstream->Write(&infoHeader,1,sizeof(BITMAPINFOHEADER));

	dword row = width * 3;
	dword alig = row % 4;
	dword zero = 0;

	y = height;
	while(y--)
	{
		byte* rowstart = getpixel(0,y);
		bmpstream->Write(rowstart,1,row);
		
		if (alig != 0)
		{
			bmpstream->Write(&zero,1,alig);
		}
	}

	if(closestream)
	{
		bmpstream->Close();
	}
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
