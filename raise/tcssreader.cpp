#include "stdafx.h"
#include "tcssreader.h"


TCSSReader::TCSSReader( TStream* readStream )
{
	BaseStream = readStream;
	TextStream = new TStreamReader ( readStream, (TEncoding&)Encodings.UTF8 );
}

void TCSSReader::Parse( bool closeStream /*= true*/ )
{

}
