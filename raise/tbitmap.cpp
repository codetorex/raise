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

TBitmap::TBitmap(int _width,int _height, TBufferFormat* _format)
{
	create(_width,_height,_format);
}

void TBitmap::create(int _width,int _height, TBufferFormat* _format)
{

	format = _format;
	flags = 0;
	bits = format->BitsPerItem;
	bytes = format->BytesPerItem;

	pixels = _width * _height;
	length = pixels * bytes;
	data = new byte[length];
	width = _width;
	height = _height;

	format = _format;

	if (bytes == 4)
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
	if (data)
	{
		delete [] data;
	}
	data = 0;
	format = 0;
	length = width = height = pixels = bits = bytes = log2width = 0;
}

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
	
	format = TBitmapFormats::fBGR;
	if(infoHeader.biBitCount == 32)
	{
		format = TBitmapFormats::fRGBA;
	}

	create(infoHeader.biWidth,infoHeader.biHeight,format);

	int rowWidth = infoHeader.biWidth * bytes; // row width for file, has padding
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

	if (convertRGB && format == TBitmapFormats::fBGR)
	{
		convert(TBitmapFormats::fRGB);
	}

	if (closestream)
	{
		bmpstream->Close();
	}
}



void TBitmap::convert(TBufferFormat* _newformat)
{
	if (format == _newformat)
		return;

	if (format == TBitmapFormats::fBGR)
	{
		if (_newformat == TBitmapFormats::fRGB)
		{
			byte* src = data;
			int curpixel = pixels;
			byte tmp;
			while(curpixel--)
			{
				tmp = *src;
				src[0] = src[2];
				src[2] = tmp;
				src += 3;
			}
			format = _newformat; // conversion done.
			return;
		}
	}

	// TODO: implement a virtual machine compiler for converting formats between

	/*TBitmapFormat fsrc(format);
	TBitmapFormat fdst(_format);

	bool sameChannels = (fsrc.chn.value == fdst.chn.value);
	bool sameType = (fsrc.typ == fdst.typ);

	

	if (sameChannels && sameType && ( 
		(fsrc.ordering == bord::rgba && fdst.ordering == bord::bgra) || 
		(fsrc.ordering == bord::bgra && fdst.ordering == bord::rgba) )) // do easy exchange.
	{
		int curpixel = pixels;
		byte tmp;
		while(curpixel--)
		{
			tmp = *src;
			src[0] = src[2];
			src[2] = tmp;
			src += fsrc.bytes;
		}
		format = _format; // conversion done.
		return;
	}*/

	/*byte srcpxl[32];
	
	byte* dst = data;

	if (fsrc.bitsperchannel == fdst.bitsperchannel)
	{
		src = data;
		if (fsrc.bitsperchannel == 8)
		{
			for (int i=0;i<pixels;i++) // for each pixels
			{
				PixelCopy(srcpxl,src,bytes); // read pixel
				src += bytes;
				fdst.translatepixel8(dst,src,fsrc);
				dst = src;
			}
		}
		else if(fsrc.bitsperchannel == 32)
		{
			for (int i=0;i<pixels;i++) // for each pixels
			{
				PixelCopy(srcpxl,src,bytes); // read pixel
				src += bytes;
				fdst.translatepixel32(dst,src,fsrc);
				dst = src;
			}
		}
		else
		{
			for (int i=0;i<pixels;i++) // for each pixels
			{
				PixelCopy(srcpxl,src,bytes); // read pixel
				src += bytes;
				fdst.translatepixel(dst,src,fsrc);
				dst = src;
			}
		}
	}*/
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

