


#ifndef TBITMAP_H
#define TBITMAP_H

#include "raisetypes.h"
#include "tstream.h"
#include "mtools.h"
#include "tbinary.h"
#include "tbitstack.h"


#ifndef LINUX
#ifndef M64
inline void PixelCopy(void* _dst,void* _src,int _size)
{
	_asm
	{
		mov esi,_src
		mov edi,_dst
		mov ecx,_size
		rep movsb
	}
}

#else

inline void PixelCopy(void* _dst,void* _src,int _size)
{
	_asm
	{
		mov rsi,_src
		mov rdi,_dst
		mov rcx,_size
		rep movsb
	}
}
#endif

#else
inline void PixelCopy(void* _dst,void* _src,int _size)
{
	asm
	(
		"cld;"
		"rep\n"
		"movsb;"
		:
		: "S"(_src),"D"(_dst),"c"(_size)
	);
}
#endif

/**
* Definitions for format
*/

struct TBitmapChannels
{
	enum echannels
	{
		red,
		green,
		blue,
		alpha,
		luminance,
		depth,
		stencil
	};
};

struct TBitmapChannelTypes
{
	enum echanneltypes
	{
		t8bit,
		t16bit,
		tfloat,
	};
};

struct TBitmapChannelOrdering
{
	enum echannelordering
	{
		rgba,
		bgra,
		abgr,
		argb,
	};
};


typedef TBitmapChannels			bchn;
typedef TBitmapChannelTypes		btyp;
typedef TBitmapChannelOrdering	bord;

/**
* Definitions for format
* 8 bits are for channels available
* 4 bits are for per channel data type
* 4 bits are for channel ordering
* 4 bits are for optimization flags
* 20 bits used and 12 bits are avilable for future usage
*/

#define MAXCHANNELS		8

#define FRED			1
#define FGREEN			2
#define FBLUE			4
#define FRGB			(FRED | FGREEN | FBLUE)
#define FALPHA			8
#define FRGBA			(FRGB | FALPHA)
#define FLUM			16
#define FDEPTH			32
#define FSTENCIL		64
#define FDEPTHSTENCIL	(FDEPTH | FSTENCIL)

// maximum 16 different types are supported (4-bit)
#define F8BIT			(1 << 8)	// 8 bit per channel
#define F16BIT			(2 << 8)	// 16 bit per channel NOT IMPLEMENTED MAYBE NEVER WILL BE?
#define FFLOAT			(3 << 8)	// float per channel

// channel ordering
// so user not confused with (CRGB | CBGR),
// he will just use CBGR and channel bits will set // 0 is RGBA
#define FBGR			((1 << 12) | FRGB)		// actually not a new ordering
#define FBGRA			((1 << 12) | FRGBA)
#define FABGR			((2 << 12) | FRGBA)
#define FARGB			((3 << 12) | FRGBA)

// optimization flags 4 bits, 2 reserved
#define O2NWIDTH		(1 << 16)	// Image is 2nwidth
#define O32BITALIGN		(2 << 16)	// Image uses 4 bytes per pixel

/* When needed in future
#define FCMYK			(1 << 20)
#define FHSV			(2 << 20)
*/

/**
* Initial bitmap class.
*/
class RDLL TBitmap
{
public:
	byte* data;
	dword length;

	int width;
	int height;
	int pixels;
	int bits;	// bits per pixel
	int bytes;	// bytes per pixel
	int log2width; // log base 2 of width
	dword format;

	TBitmap();
	TBitmap(int _width,int _height, dword _format);


	/**
	* This function is used for creating buffer of bitmap.
	* @param _width width of bitmap
	* @param _height height of bitmap
	* @param _bpp bits per pixel ( for RGB use 24, for RGBA 32 )
	*/
	void create(int _width,int _height, dword _format);
	
	/**
	* Convert current bitmap format to another.
	* @param _format new bitmap format
	*/
	void convert(dword _format);

	//TODO: you can implement, copy convert, new convert, and copy funcs.

	void release();

	/**
	* Generic getpixel function that works for all implementations.
	* Its faster but unsafe.
	*/
	inline byte* getpixel(int x,int y)
	{
		return (data + (bytes*((y*width) + x)));
	}

	/**
	* Generic setpixel function that works for all implementations.
	* Its faster but unsafe.
	*/
	inline void setpixel(int x,int y,byte* clr)
	{
		byte* ldata = getpixel(x,y);
		PixelCopy(ldata,clr,bytes);
	}

	/**
	* Optimized pixel fetch for 2^n width bitmaps. 1 mul less.
	*/
	inline byte* getpixel2n(int x,int y)
	{
		return (data + (bytes*((y<<log2width) + x)));
	}


	/**
	* Optimized pixel fetch for 2^n width bitmaps. 1 mul less.
	*/
	inline void setpixel2n(int x,int y,byte* clr)
	{
		byte* ldata = getpixel2n(x,y);
		PixelCopy(ldata,clr,bytes);
	}

		/**
	* Optimized pixel fetch for 32bpp bitmaps. 1mul less.
	*/
	inline byte* getpixel32(int x,int y)
	{
		return (data + (MUL2((y*width) + x)) );
	}

	/**
	* Optimized pixel set for 32bpp bitmaps.
	*/
	inline void setpixel32(int x,int y,byte* clr)
	{
		byte* ldata = getpixel32(x,y);
		*(dword*)ldata = *(dword*)clr;
	}

	/**
	* Optimized clearing function for 32bpp bitmaps.
	*/
	inline void clear32(byte* clr)
	{
		dword* tbytes = (dword*)data;// temp byte ptr
		int pc = pixels; // temp pixel count 
		dword px = *(dword*)clr;
		while(pc--)
		{
			*tbytes = px;
			tbytes++;
		}
	}

	/**
	* Optimized pixel fetch for 32bpp bitmaps. 1mul less.
	*/
	inline byte* getpixel2n32(int x,int y)
	{
		return (data + (MUL2((y<<log2width) + x)));
	}

	/**
	* Optimized pixel set for 32bpp bitmaps.
	*/
	inline void setpixel2n32(int x,int y,byte* clr)
	{
		byte* ldata = getpixel(x,y);
		*(dword*)ldata = *(dword*)clr;
	}

	/**
	* Generic clearing function.
	*/
	inline virtual void clear(byte* clr)
	{
		byte* tbytes = data;// temp byte ptr
		int pc = pixels; // temp pixel count 
		while(pc--)
		{
			PixelCopy(tbytes,clr,bytes);
			tbytes += bytes;
		}
	}


	// TOOLS Here
	void drawline			(int x,int y,int x2,int y2,byte* pClr);
	void wideline			(int x,int y,int x2,int y2,byte thick,byte* pClr);
	void perp				(int x, int y, int x2, int y2, byte thick,byte* pClr);//perpendicular
	void filltriangle		(int x1,int y1,int x2,int y2,int x3,int y3,byte* pClr);
	void drawcircle			(int x,int y,int radius,byte* pClr);

	void imagecopy			(TBitmap* src,int fx,int fy,int tx,int ty,int w,int h); // from x,y and to x,y then width,height.
	void imagetile			(TBitmap* src,int fx,int fy,int fw,int fh,int tx,int ty,int tw,int th);
	void blend				(TBitmap* dest,float tween = 0.5f);
	void additive			(TBitmap* dest);

	void resize				(int dwidth,int dheight);
	void colorkey			(byte* clr,byte alpha = 0); // converts clr to alpha
	void monoalpha			(); // converts monochrome version of image to alpha channel


	void matrixmul			(float m[],int mw,int mh,bool mabs = true);	// matrix multiplicion
	void sharpen			();
	void emboss				();
	void edged				();
	void blur				();

	void monochrome			();
	void histogram			();
	void invert				();

	void cellular			(int pc,long sclr,int style,bool ast);
	void perlinnoise		(float z,float scal=16.0f);

	byte* getaverage		();

	// Load Save functions


	/**
	* Loading bitmap files from stream.
	* @param bmpstream stream to read files
	* @param toRGB bmp files are stored in BGR format, if set true automatically converted to RGB
	* @param closestream closes the stream after reading done
	*/
	void loadbmp(Stream* bmpstream,bool toRGB,bool closestream = true);
	void savebmp(Stream* bmpstream,bool closestream = true);
};

class TBitmapFormat
{
public:
	bindw chn; // channel bits like FRED FGREEN FRGB
	int channelCount;
	int bitsperchannel;
	int bytesperchannel;
	int bits;
	int bytes;
	int typ; // flag that knows is 8bit per channel or 16 or float

	int ordering; // ordering from TBitmapChannelOrdering or bord

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
		chn = bmpbs.pop(8);
		typ = bmpbs.pop(4);
		ordering = bmpbs.pop(4);

		channelCount = chn.popcount();

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

		bits = channelCount * bitsperchannel;
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
		int c=fsrc.channelCount;
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
		int c=fsrc.channelCount;
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
		int c=fsrc.channelCount;
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

#endif