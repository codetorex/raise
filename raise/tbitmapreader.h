#ifndef TBITMAPREADER_H
#define TBITMAPREADER_H

#include "tarray.h"

class TBitmap;
typedef class TStream Stream;

class TBitmapReader
{
public:
	ui32 ReadExtension;
	virtual void ReadBitmap(TBitmap* bmp, Stream* src) = 0;

	static Array< TBitmapReader* > Readers;

	/**
	 * Every instance of a reader automatically adds itself to readers list
	 */
	TBitmapReader()
	{
		Readers.Add(this);
	}

	static TBitmapReader* GetReader(ui32 extension)
	{
		int i= Readers.Count;
		while(i--)
		{
			if (Readers.Items[i]->ReadExtension == extension)
			{
				return Readers.Items[i];
			}
		}
		return 0;
	}
};

#endif