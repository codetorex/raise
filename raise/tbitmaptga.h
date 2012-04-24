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

		Readers.Add(this);
		Writers.Add(this);
	}

	void WriteBitmap(TBitmap* bmp, Stream* dst, TBitmapWriterParameters* params = 0);

	void ReadBitmap(TBitmap* bmp, Stream* src);

};

#endif