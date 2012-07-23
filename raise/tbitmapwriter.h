#ifndef TBITMAPWRITER_H
#define TBITMAPWRITER_H

#include "tbitmapwriterparameters.h"

class TBitmap;
typedef class TStream Stream;

class TBitmapWriter
{
public:
	ui32 WriteExtension;
	virtual void WriteBitmap(TBitmap* bmp, Stream* dst, TBitmapWriterParameters* prm = 0) = 0;

	static TArray< TBitmapWriter* > Writers;

	/**
	 * Every instance of a writer automatically adds itself to writers list
	 */
	TBitmapWriter()
	{
		Writers.Add(this);
	}

	static TBitmapWriter* GetWriter(ui32 extension)
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

