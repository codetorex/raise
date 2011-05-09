#ifndef TBITMAPWRITER_H
#define TBITMAPWRITER_H

#include "tarray.h"

class TBitmap;
typedef class TStream Stream;

class TBitmapWriter
{
public:
	dword WriteExtension;
	virtual void WriteBitmap(TBitmap* bmp, Stream* dst) = 0;

	static TArray< TBitmapWriter* > Writers;

	static TBitmapWriter* GetWriter(dword extension)
	{
		int i= Writers.Count;
		while(i--)
		{
			if (Writers.Item[i]->WriteExtension == extension)
			{
				return Writers.Item[i];
			}
		}
		return 0;
	}
};

#endif

