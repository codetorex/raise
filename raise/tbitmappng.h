#ifndef TBITMAPPNG_H
#define TBITMAPPNG_H

#include "tbitmapreader.h"
#include "tbitmapwriter.h"

#ifndef NOPNGLIB

class TBitmapPNG: public TBitmapReader, public TBitmapWriter
{
public:
	TBitmapPNG()
	{
		WriteExtension = ReadExtension = DWORDSTR(".PNG");
	}

	void ReadBitmap(TBitmap* bmp, Stream* src);

	void WriteBitmap(TBitmap* bmp, Stream* dst, TBitmapWriterParameters* prm = 0);
};

#endif

#endif