#ifndef TBITMAPFORMATS_H
#define TBITMAPFORMATS_H

#include "tcompositionmanager.h"

// TODO: make these like B�tmapChannel manager, a singleton that manages better than static definitions?

class TBitmapFormats: public TCompositionManager
{
public:

	TCompositionPrimitive cRed;
	TCompositionPrimitive cGreen;
	TCompositionPrimitive cBlue;
	TCompositionPrimitive cAlpha;

	// TODO: move float stuff here

	TBufferFormat* fARGB;
	TBufferFormat* fABGR;
	TBufferFormat* fRGBA;
	TBufferFormat* fBGRA;
	TBufferFormat* fRGB;
	TBufferFormat* fBGR;

	void InitializeBitmapFormats();
	void CreateDefaultConverters();

	static void CreateDefaultFormats();
	
	TBitmapFormats(TCompositeFormatFactory* _factory = 0): TCompositionManager(_factory)
	{
		InitializeBitmapFormats();
		CreateDefaultConverters();
	}
};

/**
 * This pointer should be initialized before using bitmap and graphics stuff.
 */
extern TBitmapFormats* BitmapFormats;

#endif