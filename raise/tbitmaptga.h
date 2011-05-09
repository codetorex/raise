#ifndef TBITMAPTGA_H
#define TBITMAPTGA_H

#include "tbitmapreader.h"
#include "tbitmapwriter.h"

class TBitmapTGA: public TBitmapReader, public TBitmapWriter
{
public:
	TBitmapTGA()
	{
		WriteExtension = ReadExtension = DWORDSTR(".TGA");
	}

	void WriteBitmap(TBitmap* bmp, Stream* dst);

	void ReadBitmap(TBitmap* bmp, Stream* src);

	static void Install();
};

#endif