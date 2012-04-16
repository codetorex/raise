#ifndef TGRAPHICS_H
#define TGRAPHICS_H

#include "tcolor.h"
#include "tbitmap.h"

class TPen
{
public:
	TColor32 Color;
	float Width;

	TPen( TColor32 _color, float _width = 1.0f)
	{
		Color = _color;
		Width = _width;
	}

	// dash options?
	// end cap, start cap?
};

class TBrush
{
public:
	// there is different types of brushes
	// like solidBrush, TextureBrush and linearGradient brush
	// we going to act like solid we may improve this later.

	TColor32 Color;

	TBrush( TColor32 _color )
	{
		Color = _color;
	}
};

class TFont
{
public:

};

/**
 * Somewhat TBitmapBuilder lol or TCanvas?
 * http://msdn.microsoft.com/en-us/library/system.drawing.graphics.aspx
 * This going to be adapter for different bitmap types.
 */
class TGraphics
{
public:

	virtual void Translate(int x, int y) = 0;

	virtual void ResetTransform() = 0;

	virtual TColor32 GetPixel(int x, int y) = 0;

	virtual void SetPixel(int x, int y, TColor32 c) = 0;

	virtual void DrawLine(TPen& pen, int x1, int y1, int x2, int y2) = 0;
	
	virtual void FillRectangle(TBrush& brush, int x, int y, int width, int height) = 0;

	virtual void DrawHorizontalLine(TPen& pen, int x, int y, int width) = 0;

	virtual void DrawVerticalLine(TPen& pen, int x, int y, int height) = 0;

	virtual void DrawRectangle(TPen& pen, int x, int y, int width, int height) = 0;

	//virtual void DrawString(const TString& s, TFont& font, TBrush& brush, float x, float y) = 0;



	/*
	THIS CLASS SHOULD IMPLEMENT STUFF LIKE THIS

	// TOOLS Here
	void DrawLine			(int x,int y,int x2,int y2,byte* pClr);
	void wideline			(int x,int y,int x2,int y2,byte thick,byte* pClr);
	void perp				(int x, int y, int x2, int y2, byte thick,byte* pClr);//perpendicular
	void filltriangle		(int x1,int y1,int x2,int y2,int x3,int y3,byte* pClr);
	void drawcircle			(int x,int y,int radius,byte* pClr);

	void DrawRectangle		(int x,int y,int _width,int _height,byte* pClr);
	
	void tile				(TBitmap* src,int fx,int fy,int fw,int fh,int tx,int ty,int tw,int th);
	void blend				(TBitmap* dest,float tween = 0.5f);
	void additive			(TBitmap* dest);

	void resize				(int dwidth,int dheight);
	*/

	static TGraphics* FromBitmap(TBitmap* bitmap);

};


/**
 * only ARGB bitmaps supported
 */
class TBitmapGraphics: public TGraphics
{
private:
	void DrawHorizontalLineAdditive(TPen& pen, int x, int y, int width)
	{
		throw NotImplementedException();
	}

	void DrawVerticalLineAdditive(TPen& pen, int x, int y, int height)
	{

	}

	void DrawLineAdditive(TPen& pen, int x1, int y1, int x2, int y2)
	{

	}

	inline void TranslateCoord(int& x, int& y)
	{
		x += Translation.x;
		y += Translation.y;
	}

public:
	TBitmap* Bitmap;
	vec2i Translation;

	TBitmapGraphics(TBitmap* _bitmap)
	{
		Bitmap = _bitmap;
		ResetTransform();
	}

	/**
	 * This function sets a pixel on underlying graphics.
	 * Coordinates given will not be translated!
	 */
	inline void SetPixel(int x, int y, TColor32 c)
	{
		Bitmap->SetPixel(x,y,c);
	}

	inline TColor32 GetPixel(int x, int y)
	{
		return TColor32(*(ui32*)Bitmap->GetPixel(x,y));
	}

	void DrawLine(TPen& pen, int x1, int y1, int x2, int y2);

	inline void Translate(int x, int y)
	{
		Translation.set(x,y);
	}

	inline void ResetTransform()
	{
		Translate(0,0);
	}

	inline void DrawHorizontalLine(TPen& pen, int x, int y, int width)
	{
		TranslateCoord(x,y);

		ui32* dst = (ui32*)Bitmap->GetPixel(x,y);

		for (int i=0;i<width;i++)
		{
			*(dst++) = pen.Color;
		}
	}

	inline void FillRectangle(TBrush& brush, int x, int y, int width, int height)
	{
		TranslateCoord(x,y);

		int limit = y + height;
		TPen pn(brush.Color); // other modes are not implemented yet
		for (int my = y; my < limit; my++)
		{
			DrawHorizontalLine(pn,x,my,width);
		}
	}

	inline void DrawVerticalLine(TPen& pen, int x, int y, int height)
	{
		TranslateCoord(x,y);

		ui32* dst = (ui32*)Bitmap->GetPixel(x,y);
		for (int i=0;i<height;i++)
		{
			*(dst) = pen.Color;
			dst += Bitmap->Width;
		}
	}

	inline void DrawRectangle(TPen& pen, int x, int y, int width, int height)
	{
		TranslateCoord(x,y);

		DrawHorizontalLine(pen,x,y,width);
		DrawVerticalLine(pen,x,y,height);
		DrawHorizontalLine(pen,x,y+height,width);
		DrawVerticalLine(pen,x+width,y,height);
	}
};


/*class TBitmap32Bpp2nGraphics: public TBitmapGraphics
{
public:
	int LogWidth; // log base 2 of width

	TBitmap32Bpp2nGraphics(TBitmap* _bitmap, int _logwidth): TBitmapGraphics(_bitmap)
	{
		LogWidth = _logwidth;
	}

		
	inline byte* GetPixel2(int x,int y)
	{
		return (Data + (BufferFormat->BytesPerItem*((y<<LogWidth) + x)));
	}

	inline void SetPixel2(int x,int y,byte* clr)
	{
		byte* ldata = GetPixel2(x,y);
		MemoryDriver::ShortCopy(ldata,clr,BufferFormat->BytesPerItem);
	}
	
	inline byte* GetPixel32(int x,int y)
	{
		return (Data + (MUL2((y*Width) + x)) );
	}

	inline void SetPixel32(int x,int y,byte* clr)
	{
		byte* ldata = GetPixel32(x,y);
		*(ui32*)ldata = *(ui32*)clr;
	}
	

	inline TColor32 GetPixel(int x, int y)
	{
		ui32 pixelOffset = (y<<LogWidth) + x;
		ui32 byteOffset = pixelOffset * 4;
		return TColor32(*(ui32*)(Bitmap->Data + byteOffset));
	}

	inline void SetPixel(int x, int y,TColor32 c)
	{
		byte* ldata = Bitmap->GetPixel(x,y);
		*(ui32*)ldata = c;
	}
};*/

/*void wideline			(int x,int y,int x2,int y2,byte thick,byte* pClr);
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

	void ColorKey			(const TColor24& clr,byte alpha = 0); // converts clr to alpha
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

	void FlipHorizontal		();

	void FlipVertical		();*/

#endif