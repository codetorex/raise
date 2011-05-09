#include "stdafx.h"
#include "tstreamreader.h"

template <> 
void TStreamReader<TBufferedStream>::LoadStream(TStream* stream,int bufferSize)
{
	BaseStream = stream;
	BufSource = new TBufferedStream (stream,bufferSize);
	EndOfStream = false;
}

template <> 
void TStreamReader<TMemoryStream>::LoadStream(TStream* stream,int bufferSize)
{
	BaseStream = stream;
	BufSource = new TMemoryStream (stream);
	EndOfStream = false;
}