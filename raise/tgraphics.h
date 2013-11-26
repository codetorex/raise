#ifndef TGRAPHICS_H
#define TGRAPHICS_H

#include "tcolor.h"
#include "tbitmap.h"
#include "tblend.h"

class TPen
{
public:
	TColor32 Color;
	float Width;

	TPen( const TColor32& _color, float _width = 1.0f)
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

	TBrush( const TColor32& _color )
	{
		Color = _color;
	}
};

class TFont
{
public:
	String FamilyName;
	int Size;
};


/**
 * Create a class named Drawable or something with draw function that takes a TGraphics as input
 * so we can abstractise TBitmap usage in here and can use this TGraphics interface as VDraw
 */

/**
 * Somewhat TBitmapBuilder lol or TCanvas?
 * http://msdn.microsoft.com/en-us/library/system.drawing.graphics.aspx
 * This going to be adapter for different bitmap types.
 */
class TGraphics
{
public:
	virtual void SetBlending(TBlendMode* op) = 0;

	virtual TBlendMode* GetBlending() = 0;

	virtual void Translate(int x, int y) = 0;

	virtual void ResetTransform() = 0;

	virtual TColor32 GetPixel(int x, int y) = 0;

	virtual void SetPixel(int x, int y, TColor32 c) = 0;

	virtual void DrawLine(TPen& pen, int x1, int y1, int x2, int y2) = 0;
	
	virtual void FillRectangle(TBrush& brush, int x, int y, int width, int height) = 0;

	virtual void DrawHorizontalLine(TPen& pen, int x, int y, int width) = 0;

	virtual void DrawVerticalLine(TPen& pen, int x, int y, int height) = 0;

	virtual void DrawRectangle(TPen& pen, int x, int y, int width, int height) = 0;

	virtual void DrawEllipse(TPen& pen, int x, int y, int width, int height) = 0;

	virtual void DrawImage( TBitmap& bmp, int dstX, int dstY, int srcX, int srcY, int width, int height ) = 0;

	virtual void FlipHorizontal() = 0;

	virtual void FlipVertical() = 0;

	virtual void SetClip(const IRectangle& rect) = 0;

	virtual void TranslateClip(int x, int y) = 0;

	virtual void ResetClip() = 0;

	inline void DrawImage2(TBitmap& bmp, int dstX, int dstY, int srcX, int srcY)
	{
		DrawImage(bmp,dstX,dstY,srcX,srcY,bmp.Width,bmp.Height);
	}

	inline void DrawImage2(TBitmap& bmp, int x , int y)
	{
		DrawImage2(bmp,x,y,0,0);
	}

	inline void DrawImage2(TBitmap& bmp, IPosition& pos)
	{
		DrawImage2(bmp,pos.X,pos.Y,0,0);
	}

	inline void FillRectangle2( TBrush& brush, const IRectangle& rect)
	{
		FillRectangle(brush,rect.X,rect.Y,rect.Width,rect.Height);
	}

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
 * only RGBA bitmaps supported
 */
class TBitmapGraphics: public TGraphics
{
private:
	inline void TranslateCoord(int& x, int& y)
	{
		x += Translation.X;
		y += Translation.Y;
	}

	inline void ClampCoord(int& x, int& y)
	{
		x = Math.Clamp<int>(0, Bitmap->Width-1, x);
		y = Math.Clamp<int>(0, Bitmap->Height-1,y);
	}

	inline void TranslateClampCoord(int& x , int& y)
	{
		TranslateCoord(x,y);
		ClampCoord(x,y);
	}

	inline void SetPixelDiscarding(int x,int y, const TColor32& c )
	{
		if (x < 0 || y < 0) return;
		if (x > (Bitmap->Width-1) || y > (Bitmap->Height-1)) return;

		byte* p = Bitmap->GetPixel(x,y);
		*(ui32*)p = c.color;
	}

public:
	TBitmap* Bitmap;
	IPosition Translation;
	TBlendMode* BlendMode;

	IRectangle ClipRect;


	TBitmapGraphics()
	{
		SetBlending( &TBlendModes::Copy );
	}

	TBitmapGraphics(TBitmap* _bitmap)
	{
		Initialize(_bitmap);
		SetBlending(&TBlendModes::Copy);
	}

	void SetClip(const IRectangle& rect)
	{
		ClipRect = rect;
	}

	void ResetClip()
	{
		ClipRect.SetRectangle(0,0,Bitmap->Width,Bitmap->Height);
	}

	void TranslateClip(int x, int y)
	{
		ClipRect.TranslateVector(x,y);

		if (ClipRect.Right() > Bitmap->Width)
		{
			ClipRect.Width = Bitmap->Width - ClipRect.X;
		}

		if (ClipRect.Bottom() > Bitmap->Height)
		{
			ClipRect.Height = Bitmap->Height - ClipRect.Y;
		}
	}

	inline void SetBlending(TBlendMode* op)
	{
		BlendMode = op;
	}

	inline TBlendMode* GetBlending()
	{
		return BlendMode;
	}

	inline void Initialize(TBitmap* _bitmap)
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
		BlendMode->Blend(c,*(TColor32*)Bitmap->GetPixel(x,y));
	}

	inline TColor32 GetPixel(int x, int y)
	{
		return TColor32(*(ui32*)Bitmap->GetPixel(x,y));
	}

	void DrawLine(TPen& pen, int x1, int y1, int x2, int y2);

	inline void Translate(int x, int y)
	{
		Translation.SetVector(x,y);
	}

	inline void ResetTransform()
	{
		Translate(0,0);
	}

	inline void DrawHorizontalLine(TPen& pen, int x, int y, int width)
	{
		TranslateClampCoord(x,y);

		if (width > Bitmap->Width)
		{
			width = Bitmap->Width;
		}

		ui32* dst = (ui32*)Bitmap->GetPixel(x,y);

		for (int i=0;i<width;i++)
		{
			*(dst++) = pen.Color;
		}
	}

	inline void FillRectangle(TBrush& brush, int x, int y, int width, int height)
	{
		int limit = y + height;
		TPen pn(brush.Color); // other modes are not implemented yet
		for (int my = y; my < limit; my++)
		{
			DrawHorizontalLine(pn,x,my,width);
		}
	}

	void DrawDottedVerticalLine(const TColor32& color, int x, int y, int height)
	{
		TranslateClampCoord(x,y);
		int ender = y + height;
		for(y++;y<ender;y+=2)
		{
			SetPixel(x,y,color);
		}
	}

	void DrawDottedHorizontalLine(const TColor32& color, int x, int y, int width)
	{
		TranslateClampCoord(x,y);
		int ender = x + width;

		for(x++;x<ender;x+=2)
		{
			SetPixel(x,y,color);
		}
	}

	inline void DrawVerticalLine(TPen& pen, int x, int y, int height)
	{
		TranslateClampCoord(x,y);

		if (height > Bitmap->Height)
		{
			height = Bitmap->Height;
		}

		ui32* dst = (ui32*)Bitmap->GetPixel(x,y);
		for (int i=0;i<height;i++)
		{
			*(dst) = pen.Color;
			dst += Bitmap->Width;
		}
	}

	inline void DrawRectangle(TPen& pen, int x, int y, int width, int height)
	{
		DrawHorizontalLine(pen,x,y,width);
		DrawVerticalLine(pen,x,y,height);
		DrawHorizontalLine(pen,x,y+height,width);
		DrawVerticalLine(pen,x+width,y,height);
	}

	/*void DrawImage(TBitmap& bmp, TPosition& pos)
	{
		DrawImage(bmp,pos.X,pos.Y,0,0);
	}*/

	void DrawEllipse(TPen& pen, int x, int y, int width, int height);

	void DrawImage(TBitmap& bmp, int dstX, int dstY, int srcX, int srcY, int width, int height );

	void FlipHorizontal();

	void FlipVertical();

	void Clear(const TColor32& color);
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