#include "stdafx.h"
#include "tbitmap.h"
#include "tbinary.h"
#include "tbitfield.h"
#include "tbitstack.h"

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



class TBitmapFormat
{
public:
	int channels;
	int bitsperchannel;
	int bytesperchannel;
	int bits;
	int bytes;
	int typ;

	int ordering;

	int loc[MAXCHANNELS]; // channel locations
	int chns[MAXCHANNELS]; // channels by location. index is location, value is channel

	int log2chn;

	TBitmapFormat(dword format)
	{
		loadformat(format);
		for(int i=0;i<MAXCHANNELS;i++) loc[i] = -1;
	}

	static void TransposeIntArray(int* dst,int* src,int length)
	{
		for (int i=0;i<length;i++)
		{
			for (int k=0;k<length;k++)
			{
				if (src[k] == i)
				{
					dst[i] = k;
					break;
				}
			}
		}
	}

	void getlocations()
	{
		TransposeIntArray(chns,loc,MAXCHANNELS);
	}

	void loadformat(dword format)
	{
		TBitstack bmpbs = format;
		// I converted this from bitfield to bitstack
		bindw chn = bmpbs.pop(8);
		typ = bmpbs.pop(4);
		ordering = bmpbs.pop(4);

		channels = chn.popcount();

		switch(typ)
		{
		case 0: // not set so 8 bit
		case 1: // 8 bit
			bitsperchannel = 8;
			break;

		case 2: // 16 bit
			bitsperchannel = 16;
			break;

		case 3: // float
			bitsperchannel = 32;
			break;
		}

		bits = channels * bitsperchannel;
		bytes = DIV8(bits);
		bytesperchannel = DIV8(bitsperchannel);
		
		bindw spc = bitsperchannel;
		log2chn = spc.log2k();

		if ( (format & FRGB) > 0 )
		{
			switch(ordering)
			{
			case bord::bgra:
				loc[bchn::blue] = 0;
				loc[bchn::green] = 1;
				loc[bchn::red] = 2;
				loc[bchn::alpha] = 3;
				break;

			case bord::rgba:
				loc[bchn::red] = 0;
				loc[bchn::green] = 1;
				loc[bchn::blue] = 2;
				loc[bchn::alpha] = 3;
				break;

			case bord::abgr:
				loc[bchn::alpha] = 0;
				loc[bchn::blue] = 1;
				loc[bchn::green] = 2;
				loc[bchn::red] = 3;
				break;

			case bord::argb:
				loc[bchn::alpha] = 0;
				loc[bchn::red] = 1;
				loc[bchn::green] = 2;
				loc[bchn::blue] = 3;
				break;
			}

			if ( (format & FRED) == 0 ) loc[bchn::red] = -1;
			if ( (format & FGREEN) == 0 ) loc[bchn::green] = -1;
			if ( (format & FBLUE) == 0 ) loc[bchn::blue] = -1;
			if ( (format & FALPHA) == 0 ) loc[bchn::alpha] = -1;
		}
		else if( format & FDEPTH)
		{
			loc[bchn::depth] = 0;
		}
		else if(format & FSTENCIL)
		{
			loc[bchn::stencil] = 0;
		}
		else if( (format & FDEPTHSTENCIL) == FDEPTHSTENCIL)
		{
			loc[bchn::depth] = 0;
			loc[bchn::stencil] = 1;
		}
		else if(format & FLUM)
		{
			loc[bchn::luminance] = 0;
		}
	}

	/**
	* Flexible function for that translates pixel from this format to another format.
	* @param dst destination pixel which going to be format of this class
	* @param src source pixel which should be in format of fsrc
	* @param fsrc source pixel format
	*/
	inline void translatepixel(byte* dst,byte* src,const TBitmapFormat& fsrc)
	{
		int c=fsrc.channels;
		while (c--) // for every source channel
		{
			int myIndex = loc[ fsrc.chns[c] ] << log2chn;
			PixelCopy(dst + myIndex, src, bytesperchannel);
			src += bytesperchannel;
		}
	}

	/**
	* Translates pixel from this format to another format. Where both of them 8 bits per channel.
	* @param dst destination pixel which going to be format of this class
	* @param src source pixel which should be in format of fsrc
	* @param fsrc source pixel format
	*/
	inline void translatepixel8(byte* dst,byte* src,const TBitmapFormat& fsrc)
	{
		int c=fsrc.channels;
		while (c--) // for every source channel
		{
			int myIndex = loc[ fsrc.chns[c] ];
			dst[myIndex] = *src;
			src++;
		}
	}

	/**
	* Translates pixel from this format to another format. Where both of them 32 bits per channel.
	* @param dst destination pixel which going to be format of this class
	* @param src source pixel which should be in format of fsrc
	* @param fsrc source pixel format
	*/
	inline void translatepixel32(dword* dst,dword* src,const TBitmapFormat& fsrc)
	{
		int c=fsrc.channels;
		while (c--) // for every source channel
		{
			int myIndex = loc[ fsrc.chns[c] ];
			dst[myIndex] = *src;
			src++;
		}
	}

	inline void translatepixel32(byte* dst,byte* src,const TBitmapFormat& fsrc)
	{
		translatepixel32((dword*)dst,(dword*)src,fsrc);
	}
};


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

TBitmap::TBitmap(int _width,int _height, dword _format)
{
	create(_width,_height,_format);
}

void TBitmap::create(int _width,int _height, dword _format)
{
	TBitmapFormat tbf(_format);

	if (tbf.bitsperchannel == 8) _format |= F8BIT;

	bits = tbf.bits;
	bytes = tbf.bytes;

	pixels = _width * _height;
	length = pixels * bytes;
	data = new byte[length];
	width = _width;
	height = _height;

	format = _format;

	if (bytes == 4)
		format |= O32BITALIGN;

	TBinary wbinary(width);
	if (wbinary.is2n())
	{
		format |= O2NWIDTH;
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
	length = width = height = pixels = bits = bytes = format = log2width = 0;
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
		throw "Unsupported BMP format";
		return;
	}

	dword frm = FBGR;
	if(infoHeader.biBitCount == 32)
	{
		frm |= FALPHA;
	}

	bool bottomUp = true;
	if (infoHeader.biHeight < 0)
	{
		bottomUp = false;
		infoHeader.biHeight = abs(infoHeader.biHeight);
	}

	release();
	create(infoHeader.biWidth,infoHeader.biHeight,frm);

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

	// TODO: convert format from BGR to RGB here.

	if (closestream)
	{
		bmpstream->Close();
	}
}



void TBitmap::convert(dword _format)
{
	if (format == _format)
		return;

	TBitmapFormat fsrc(format);
	TBitmapFormat fdst(_format);

	byte srcpxl[32];
	byte* src;
	byte* dst;

	if (fsrc.bitsperchannel == fdst.bitsperchannel)
	{
		src = data;
		for (int i=0;i<pixels;i++) // for each pixels
		{
			PixelCopy(srcpxl,src,bytes); // read pixel
			src += bytes;
			fdst.translatepixel(dst,src,fsrc);
			dst = src;
		}
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