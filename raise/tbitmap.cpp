#include "stdafx.h"
#include "tbitmap.h"
#include "tbitmapreader.h"
#include "tbitmapwriter.h"
#include "tpath.h"
#include "tfilestream.h"



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
	Create(_width,_height, BitmapFormats->fRGBA);
}

TBitmap::TBitmap( const String& path ): TCompositeBuffer()
{
	Load(path);
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

void TBitmap::Save( Stream* s, TBitmapWriter& format, TBitmapWriterParameters* params /*= 0*/ )
{
	format.WriteBitmap(this,s, params);
}

void TBitmap::Save( const String& path )
{
	ui32 ext = TPath::GetExtensionAsDword(path);
	TBitmapWriter* bw = TBitmapWriter::GetWriter(ext);
	if (bw == 0)
	{
		throw Exception("Unknown file format or extension");
	}

	TFileStream* fs = new TFileStream(path, fm_Write);

	Save(fs,*bw);

	fs->Close();
}

void TBitmap::Load( Stream* s, TBitmapReader& format )
{
	format.ReadBitmap(this,s);
}

void TBitmap::Load( const String& path )
{
	ui32 ext = TPath::GetExtensionAsDword(path);
	TBitmapReader* br = TBitmapReader::GetReader(ext);
	if (br == 0)
	{
		throw Exception("Unknown file format or extension");
	}

	TFileStream* fs = new TFileStream(path, fm_Read);

	Load(fs,*br);

	fs->Close();
}