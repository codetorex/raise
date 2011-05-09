#ifndef TBITMAPBMP_H
#define TBITMAPBMP_H

#include "tbitmapreader.h"
#include "tbitmapwriter.h"

class TBitmapBMP: public TBitmapReader, public TBitmapWriter
{
public:

	TBitmapBMP()
	{
		WriteExtension = ReadExtension = DWORDSTR(".BMP");
	}

	void ReadBitmap(TBitmap* bmp, Stream* src);
	void WriteBitmap(TBitmap* bmp, Stream* dst);

	static void Install();
};

#endif