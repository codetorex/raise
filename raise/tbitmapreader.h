#ifndef TBITMAPREADER_H
#define TBITMAPREADER_H

#include "tarray.h"

class TBitmap;
typedef class TStream Stream;

class TBitmapReader
{
public:
	dword ReadExtension;
	virtual void ReadBitmap(TBitmap* bmp, Stream* src) = 0;

	static TArray< TBitmapReader* > Readers;

	static TBitmapReader* GetReader(dword extension)
	{
		int i= Readers.Count;
		while(i--)
		{
			if (Readers.Item[i]->ReadExtension == extension)
			{
				return Readers.Item[i];
			}
		}
		return 0;
	}
};

#endif