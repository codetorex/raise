#ifndef TBITMAP_H
#define TBITMAP_H

#include "tstream.h"
#include "mtools.h"
#include "tbitmapformats.h"
#include "tcolor.h"
#include "mregion2.h"

class TBitmapWriter;
class TBitmapReader;
class TBitmapWriterParameters;

/**
* Initial bitmap class.
*/
class RDLL TBitmap: public TCompositeBuffer, public ISize
{
public:
	int PixelCount; // width * height means total pixel count

	//TBufferFormat* format;

	TBitmap();
	TBitmap(int _width,int _height, TBufferFormat* _format);
	TBitmap(int _width, int _height);
	
	/**
	 * Creates bitmap out of stream.
	 */
	TBitmap(TStream* s, TBitmapReader& format);

	/**
	 * Create bitmap out of file given in paramater 'path'.
	 */
	TBitmap(const String& path);


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
	inline void SetPixel(int x,int y,const byte* clr)
	{
		byte* ldata = GetPixel(x,y);
		MemoryDriver::ShortCopy(ldata,clr,BufferFormat->BytesPerItem);
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

	inline TBitmap* ConvertCopyBitmap(TBufferFormat* newFormat)
	{
		TBitmap* newBitmap = new TBitmap(Width,Height,newFormat);
		ConvertCopy(newBitmap);
		return newBitmap;
	}

	/**
	 * Saves bitmap into a stream with given format and parameters.
	 */
	void Save(Stream* s, TBitmapWriter& format, TBitmapWriterParameters* params = 0);

	/**
	 * Path length should be bigger than 4.
	 */
	void Save(const String& path );

	void Load(Stream* s, TBitmapReader& format);

	void Load(const String& path);
};

#endif