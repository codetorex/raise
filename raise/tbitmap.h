#ifndef TBITMAP_H
#define TBITMAP_H

#include "raisetypes.h"
#include "tstream.h"
#include "mtools.h"
#include "tbinary.h"
#include "tbitstack.h"
#include "tbitmapformats.h"
#include "tcolor.h"
#include "tregion.h"

// optimization flags 4 bits, 2 reserved
#define O2NWIDTH		(1 << 16)	// Image is 2nwidth
#define O32BITALIGN		(2 << 16)	// Image uses 4 bytes per pixel

/**
* Initial bitmap class.
*/
class RDLL TBitmap: public TCompositeBuffer, public TRange
{
public:
	int PixelCount; // width * height means total pixel count
	int LogWidth; // log base 2 of width
	ui32 Flags;
	//TBufferFormat* format;

	TBitmap();
	TBitmap(int _width,int _height, TBufferFormat* _format);


	/**
	* This function is used for creating buffer of bitmap.
	* @param _width width of bitmap
	* @param _height height of bitmap
	* @param _bpp bits per pixel ( for RGB use 24, for RGBA 32 )
	*/
	void Create(int _width,int _height, TBufferFormat* _format);

	void Release();

	/**
	* Generic getpixel function that works for all implementations.
	* Its faster but unsafe.
	*/
	inline byte* GetPixel(int x,int y)
	{
		return (Data + (BufferFormat->BytesPerItem*((y*Width) + x)));
	}

	/**
	* Generic setpixel function that works for all implementations.
	* Its faster but unsafe.
	*/
	inline void SetPixel(int x,int y,byte* clr)
	{
		byte* ldata = GetPixel(x,y);
		MemoryDriver::ShortCopy(ldata,clr,BufferFormat->BytesPerItem);
	}

	/**
	* Optimized pixel fetch for 2^n width bitmaps. 1 mul less.
	*/
	inline byte* GetPixel2(int x,int y)
	{
		return (Data + (BufferFormat->BytesPerItem*((y<<LogWidth) + x)));
	}


	/**
	* Optimized pixel fetch for 2^n width bitmaps. 1 mul less.
	*/
	inline void SetPixel2(int x,int y,byte* clr)
	{
		byte* ldata = GetPixel2(x,y);
		MemoryDriver::ShortCopy(ldata,clr,BufferFormat->BytesPerItem);
	}

		/**
	* Optimized pixel fetch for 32bpp bitmaps. 1mul less.
	*/
	inline byte* GetPixel32(int x,int y)
	{
		return (Data + (MUL2((y*Width) + x)) );
	}

	/**
	* Optimized pixel set for 32bpp bitmaps.
	*/
	inline void SetPixel32(int x,int y,byte* clr)
	{
		byte* ldata = GetPixel32(x,y);
		*(ui32*)ldata = *(ui32*)clr;
	}

	/**
	* Optimized pixel fetch for 32bpp bitmaps. 1mul less.
	*/
	inline byte* GetPixel232(int x,int y)
	{
		return (Data + (MUL2((y<<LogWidth) + x)));
	}

	/**
	* Optimized pixel set for 32bpp bitmaps.
	*/
	inline void SetPixel232(int x,int y,byte* clr)
	{
		byte* ldata = GetPixel(x,y);
		*(ui32*)ldata = *(ui32*)clr;
	}

	/**
	* Generic clearing function.
	*/
	void Clear(byte* clr)
	{
		if (BufferFormat->BitsPerItem == 32)
		{
			ui32* tbytes = (ui32*)Data;// temp byte ptr
			int pc = PixelCount; // temp pixel count 
			ui32 px = *(ui32*)clr;
			while(pc--)
			{
				*tbytes = px;
				tbytes++;
			}
		}
		else
		{
			byte* tbytes = Data;// temp byte ptr
			int pc = PixelCount; // temp pixel count 
			while(pc--)
			{
				MemoryDriver::ShortCopy(tbytes,clr,BufferFormat->BytesPerItem);
				tbytes += BufferFormat->BytesPerItem;
			}
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

	inline void Copy		(TBitmap* src, TPosition* dstPos, TPosition* srcPos)
	{
		Copy(src,dstPos->X,dstPos->Y,srcPos->X,srcPos->Y,src->Width,src->Height);
	}

	inline void Copy		(TBitmap* src, TPosition* dstPos)
	{
		Copy(src,dstPos->X,dstPos->Y,0,0,src->Width,src->Height);
	}

	inline void Copy		(TBitmap* src)
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
	void ReadBMP(Stream* bmpstream,bool closestream = true);

	void WriteBMP(Stream* bmpstream,bool closestream = true);
};

#endif