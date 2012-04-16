#include "stdafx.h"
#include "tbitmap.h"

TArray<TBitmapReader*> TBitmapReader::Readers;
TArray<TBitmapWriter*> TBitmapWriter::Writers;


/*static int fieldstart[] =
{
	0,
	8,
	12,
	16,
	20,
};

static int fieldlength[] =
{
	8, // channels included
	4, // channel data types
	4, // channel ordering
	4, // optimization flags
};

#define BFCHANNEL		0
#define BFCHNTYPE		1
#define BFCHNORDR		2
#define BFOPFLAGS		3*/



TBitmap::TBitmap(): TCompositeBuffer()
{
	Width = 0;
	Height = 0;
	PixelCount = 0;
}

TBitmap::TBitmap(int _width,int _height, TBufferFormat* _format)
{
	Create(_width,_height,_format);
}

TBitmap::TBitmap( int _width, int _height )
{
	Create(_width,_height, BitmapFormats->fARGB);
}

void TBitmap::Create(int _width,int _height, TBufferFormat* _format)
{
	BufferFormat = _format;

	PixelCount = _width * _height;

	AllocateItemCapacity(PixelCount);

	Width = _width;
	Height = _height;
}

void TBitmap::Release()
{
	Free();
	Width = Height = PixelCount =  0;
}




void TBitmap::ReadBMP( Stream* bmpstream,bool closestream /*= true*/ )
{
	TBitmapReader* bmpReader = TBitmapReader::GetReader(DWORDSTR(".BMP"));
	bmpReader->ReadBitmap(this,bmpstream);
	if (closestream) bmpstream->Close();
}

void TBitmap::WriteBMP( Stream* bmpstream,bool closestream /*= true*/ )
{
	TBitmapWriter* bmpWriter = TBitmapWriter::GetWriter(DWORDSTR(".BMP"));
	bmpWriter->WriteBitmap(this,bmpstream);
	if (closestream) bmpstream->Close();
}