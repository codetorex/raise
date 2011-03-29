


#ifndef TBITMAP_H
#define TBITMAP_H

#include "raisetypes.h"
#include "tstream.h"
#include "mtools.h"
#include "tbinary.h"
#include "tbitstack.h"
#include "tflexiblebuffer.h"
#include "tcolor.h"
#include "tregion.h"

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


/**/

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

// Defines default Bitmap Channels
class TBitmapChannels
{
public:
	void CreateDefaultChannels();
	
	virtual void Initialize()
	{
		CreateDefaultChannels();
	}

	static TFormatElement* cRed;
	static TFormatElement* cGreen;
	static TFormatElement* cBlue;
	static TFormatElement* cAlpha;

	static TArray<TFormatElement*>* BitmapChannelRegistry;
};

// Defines default Bitmap Formats
class TBitmapFormats
{
public:
	static TBufferFormat* fARGB;
	static TBufferFormat* fABGR;
	static TBufferFormat* fRGBA;
	static TBufferFormat* fBGRA;
	static TBufferFormat* fRGB;
	static TBufferFormat* fBGR;

	static TArray<TBufferFormat*>* BitmapFormatRegistry;

	void CreateDefaultFormats();
	void CreateDefaultConverters();

	virtual void Initialize()
	{
		CreateDefaultFormats();
		CreateDefaultConverters();
	}
};

/**
* Initial bitmap class.
*/
class RDLL TBitmap: public TFlexibleBuffer, public TRange
{
public:
	int pixels; // width * height means total pixel count
	int log2width; // log base 2 of width
	dword flags;
	//TBufferFormat* format;

	TBitmap();
	TBitmap(int _width,int _height, TBufferFormat* _format);


	/**
	* This function is used for creating buffer of bitmap.
	* @param _width width of bitmap
	* @param _height height of bitmap
	* @param _bpp bits per pixel ( for RGB use 24, for RGBA 32 )
	*/
	void create(int _width,int _height, TBufferFormat* _format);

	void release();

	/**
	* Generic getpixel function that works for all implementations.
	* Its faster but unsafe.
	*/
	inline byte* getpixel(int x,int y)
	{
		return (Buffer + (BufferFormat->BytesPerItem*((y*Width) + x)));
	}

	/**
	* Generic setpixel function that works for all implementations.
	* Its faster but unsafe.
	*/
	inline void setpixel(int x,int y,byte* clr)
	{
		byte* ldata = getpixel(x,y);
		PixelCopy(ldata,clr,BufferFormat->BytesPerItem);
	}

	/**
	* Optimized pixel fetch for 2^n width bitmaps. 1 mul less.
	*/
	inline byte* getpixel2n(int x,int y)
	{
		return (Buffer + (BufferFormat->BytesPerItem*((y<<log2width) + x)));
	}


	/**
	* Optimized pixel fetch for 2^n width bitmaps. 1 mul less.
	*/
	inline void setpixel2n(int x,int y,byte* clr)
	{
		byte* ldata = getpixel2n(x,y);
		PixelCopy(ldata,clr,BufferFormat->BytesPerItem);
	}

		/**
	* Optimized pixel fetch for 32bpp bitmaps. 1mul less.
	*/
	inline byte* getpixel32(int x,int y)
	{
		return (Buffer + (MUL2((y*Width) + x)) );
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
		dword* tbytes = (dword*)Buffer;// temp byte ptr
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
		return (Buffer + (MUL2((y<<log2width) + x)));
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
	inline void clear(byte* clr)
	{
		byte* tbytes = Buffer;// temp byte ptr
		int pc = pixels; // temp pixel count 
		while(pc--)
		{
			PixelCopy(tbytes,clr,BufferFormat->BytesPerItem);
			tbytes += BufferFormat->BytesPerItem;
		}
	}


	// TOOLS Here
	void DrawLine			(int x,int y,int x2,int y2,byte* pClr);
	void wideline			(int x,int y,int x2,int y2,byte thick,byte* pClr);
	void perp				(int x, int y, int x2, int y2, byte thick,byte* pClr);//perpendicular
	void filltriangle		(int x1,int y1,int x2,int y2,int x3,int y3,byte* pClr);
	void drawcircle			(int x,int y,int radius,byte* pClr);

	void DrawRectangle		(int x,int y,int _width,int _height,byte* pClr);

	//void copy				(TBitmap* src,int fx,int fy,int tx,int ty,int w,int h); // from x,y and to x,y then width,height.
	
	void Copy				(TBitmap* src,int dstX,int dstY, int srcX = 0, int srcY = 0, int _width = -1,int _height = -1);
	
	inline void Copy		(TBitmap* src, TPosition* dstPos, TPosition* srcPos, TRange* size)
	{
		Copy(src,dstPos->X,dstPos->Y,srcPos->X,srcPos->Y,size->Width,size->Height);
	}

	void Copy				(TBitmap* src, TPosition* dstPos, TPosition* srcPos)
	{
		Copy(src,dstPos->X,dstPos->Y,srcPos->X,srcPos->Y,src->Width,src->Height);
	}

	void Copy				(TBitmap* src, TPosition* dstPos)
	{
		Copy(src,dstPos->X,dstPos->Y,0,0,src->Width,src->Height);
	}

	void Copy				(TBitmap* src)
	{
		Copy(src,0,0,0,0,src->Width,src->Height);
	}
	
	void tile				(TBitmap* src,int fx,int fy,int fw,int fh,int tx,int ty,int tw,int th);
	void blend				(TBitmap* dest,float tween = 0.5f);
	void additive			(TBitmap* dest);

	void resize				(int dwidth,int dheight);

	/**
	* Changes a color to alpha that specified.
	* Warning this function changes internal format to ARGB.
	*/
	void ColorKey			(const TColor24& clr,byte alpha = 0); // converts clr to alpha
	/*void monoalpha			(); // converts monochrome version of image to alpha channel


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

	byte* getaverage		();*/

	/**
	* Flips image horizontal.
	* NOTE: this function is not ready yet.
	*/
	void FlipHorizontal		();

	/**
	* Flips image vertically.
	*/
	void FlipVertical		();

	inline TBitmap* ConvertCopyBitmap(TBufferFormat* newFormat)
	{
		TBitmap* newBitmap = new TBitmap(Width,Height,newFormat);
		ConvertCopy(newBitmap);
		return newBitmap;
	}

	// Load Save functions


	/**
	* Loading bitmap files from stream.
	* @param bmpstream stream to read files
	* @param toRGB bmp files are stored in BGR format, if set true automatically converted to RGB
	* @param closestream closes the stream after reading done
	*/
	void loadbmp(Stream* bmpstream,bool toRGB,bool closestream = true);
	void savebmp(Stream* bmpstream,bool closestream = true);

	void loadtga(Stream* tgastream,bool toRGB,bool closestream = true);
	void savetga(Stream* tgastream,bool closestream = true);
};

#endif