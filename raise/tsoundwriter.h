#ifndef TSOUNDWRITER_H
#define TSOUNDWRITER_H

#include "tarray.h"

class TSound;
typedef class TStream Stream;

class TSoundWriter
{
public:
	ui32 WriteExtension;
	virtual void WriteSound(TSound* snd, Stream* dst) = 0;

	static Array< TSoundWriter* > Writers;

	/**
	 * Every instance of a writer automatically adds itself to writers list
	 */
	TSoundWriter()
	{
		Writers.Add(this);
	}

	static TSoundWriter* GetWriter(ui32 extension)
	{
		int i= Writers.Count;
		while(i--)
		{
			if (Writers.Items[i]->WriteExtension == extension)
			{
				return Writers.Items[i];
			}
		}
		return 0;
	}
};

#endif