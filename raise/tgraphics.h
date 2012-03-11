#ifndef TGRAPHICS_H
#define TGRAPHICS_H

/**
 * Somewhat TBitmapBuilder lol or TCanvas?
 * http://msdn.microsoft.com/en-us/library/system.drawing.graphics.aspx
 */
class TGraphics
{
public:
	TBitmap* Bitmap;

	TGraphics(TBitmap* Target): Bitmap(Target)
	{

	}
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



};


#endif