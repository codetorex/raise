#ifndef TSOUNDWAV_H
#define TSOUNDWAV_H

#include "tsoundreader.h"
#include "tsoundwriter.h"

class TSoundWAV: public TSoundWriter, public TSoundReader
{
public:

	TSoundWAV()
	{
		WriteExtension = ReadExtension = DWORDSTR(".WAV");
	}

	void ReadSound(TSound* snd, Stream* src);

	void WriteSound(TSound* snd, Stream* dst);

};


#endif