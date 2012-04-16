#ifndef TBITMAPFORMATS_H
#define TBITMAPFORMATS_H

#include "tcompositionmanager.h"

// TODO: make these like BÝtmapChannel manager, a singleton that manages better than static definitions?

class TBitmapFormats: public TCompositionManager
{
public:

	TCompositionPrimitive cRed;
	TCompositionPrimitive cGreen;
	TCompositionPrimitive cBlue;
	TCompositionPrimitive cAlpha;


	TBufferFormat* fARGB;
	TBufferFormat* fABGR;
	TBufferFormat* fRGBA;
	TBufferFormat* fBGRA;
	TBufferFormat* fRGB;
	TBufferFormat* fBGR;

	void InitializeBitmapFormats();
	void CreateDefaultConverters();
	void InitializeFileFormats();

	static void CreateDefaultFormats();
	
	TBitmapFormats(TCompositeFormatFactory* _factory = 0): TCompositionManager(_factory)
	{
		InitializeBitmapFormats();
		CreateDefaultConverters();
		InitializeFileFormats();
	}
};

/**
 * This pointer should be initialized before using bitmap and graphics stuff.
 */
extern TBitmapFormats* BitmapFormats;

#include "tbitmapbmp.h"
#include "tbitmaptga.h"
#include "tbitmappng.h"

#endif