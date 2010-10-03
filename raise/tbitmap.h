


#ifndef TBITMAP_H
#define TBITMAP_H

#include "raisetypes.h"
#include "tstream.h"
#include "mtools.h"


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
* 8 bits are for channels available
* 4 bits are for per channel data type
* 4 bits are for optimization flags
* 16 bits used and 16 bits are avilable for future usage
*/

#define CRED			1
#define CGREEN			2
#define CBLUE			4
#define CRGB			(CRED | CGREEN | CBLUE)
#define CALPHA			8
#define CRGBA			(CRGB | CALPHA)
#define CLUM			16
#define CDEPTH			32
#define CSTENCIL		64
#define CDEPTHSTENCIL	(CDEPTH | CSTENCIL)

#define C8BIT			(1 << 8)	// 8 bit per channel
#define C16BIT			(2 << 8)	// 16 bit per channel
#define CFLOAT			(3 << 8)	// float per channel

// optimization flags
#define O2NWIDTH		(1 << 12)	// Image is 2nwidth
#define O4BYTE			(2 << 12)	// Image uses 4 bytes per pixel


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
	int format;
	int log2width; // log base 2 of width

	TBitmap();
	TBitmap(int _width,int _height, int _format);


	/**
	* This function is used for creating buffer of bitmap.
	* @param _width width of bitmap
	* @param _height height of bitmap
	* @param _bpp bits per pixel ( for RGB use 24, for RGBA 32 )
	*/
	void create(int _width,int _height,int _bpp, int _format);

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



	void loadbmp(Stream* bmpstream,bool closestream = true);
	void savebmp(Stream* bmpstream,bool closestream = true);
};

#endif