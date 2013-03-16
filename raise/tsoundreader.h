#ifndef TSOUNDREADER_H
#define TSOUNDREADER_H

#include "tarray.h"

class TSound;
typedef class TStream Stream;

class TSoundReader
{
public:	
	ui32 ReadExtension;
	virtual void ReadSound(TSound* snd, Stream* src) = 0;

	static TArray< TSoundReader* > Readers;

	/**
	 * Every instance of a reader automatically adds itself to readers list
	 */
	TSoundReader()
	{
		Readers.Add(this);
	}

	static TSoundReader* GetReader(ui32 extension)
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