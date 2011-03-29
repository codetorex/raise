#include "stdafx.h"
#include "tbitmap.h"

#ifdef LINUX 

struct BITMAPFILEHEADER 
{
	word bfType;
	dword bfSize;
	word bfReserved1;
	word bfReserved2;
	dword bfOffBits;
} __attribute((packed));

struct BITMAPINFOHEADER
{
	dword biSize;
	int biWidth;
	int biHeight;
	word biPlanes;
	word biBitCount;
	dword biCompression;
	dword biSizeImage;
	dword biXPelsPerMeter;
	dword biYPelsPerMeter;
	dword biClrUsed;
	dword biClrImportant;
}  __attribute((packed));

#endif

void TBitmap::loadbmp(Stream* bmpstream, bool convertRGB, bool closestream /* = true */)
{
	int y;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	bmpstream->Read(&fileHeader,sizeof(BITMAPFILEHEADER),1);
	bmpstream->Read(&infoHeader,sizeof(BITMAPINFOHEADER),1);


	if (infoHeader.biCompression != 0 || (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32))
	{
		if(closestream) 
		{
			bmpstream->Close();
			bmpstream = 0;
		}
		throw Exception("Unsupported BMP format");
		return;
	}

	bool bottomUp = true;
	if (infoHeader.biHeight < 0)
	{
		bottomUp = false;
		infoHeader.biHeight = abs(infoHeader.biHeight);
	}

	release();

	BufferFormat = TBitmapFormats::fBGR;
	if(infoHeader.biBitCount == 32)
	{
		BufferFormat = TBitmapFormats::fRGBA;
	}

	create(infoHeader.biWidth,infoHeader.biHeight,BufferFormat);

	int rowWidth = infoHeader.biWidth * BufferFormat->BytesPerItem; // row width for file, has padding
	int realRowWidth = rowWidth; // row width for bitmap, doesnt has padding
	rowWidth += MOD4(rowWidth);
	byte* row = new byte[rowWidth];


	if (bottomUp)
	{
		y = Height;
		while (y--)
		{
			byte* rowstart = getpixel(0,y);
			bmpstream->Read(row,1,rowWidth);
			memcpy(rowstart,row,realRowWidth);
		}
	}
	else
	{
		for (y = 0;y<Height;y++)
		{
			byte* rowstart = getpixel(0,y);
			bmpstream->Read(row,1,rowWidth);
			memcpy(rowstart,row,realRowWidth);
		}
	}

	if (convertRGB && BufferFormat == TBitmapFormats::fBGR)
	{
		Convert(TBitmapFormats::fRGB);
	}

	if (closestream)
	{
		bmpstream->Close();
		bmpstream = 0;
	}
}


void TBitmap::savebmp( Stream* bmpstream,bool closestream /*= true*/ )
{
	int y;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = pixels * 3;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;
	infoHeader.biWidth = Width;
	infoHeader.biHeight = Height;

	fileHeader.bfSize = sizeof(BITMAPFILEHEADER);
	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmpstream->Write(&fileHeader,1,sizeof(BITMAPFILEHEADER));
	bmpstream->Write(&infoHeader,1,sizeof(BITMAPINFOHEADER));

	dword row = Width * 3;
	dword alig = row % 4;
	dword zero = 0;
	
	byte* rowstart;
	TFlexibleBuffer* newBuffer = 0;
	if (BufferFormat != TBitmapFormats::fBGR)
	{
		newBuffer = ConvertCopy(TBitmapFormats::fBGR);
		if (newBuffer == NULL)
		{
			throw Exception("No converter for this format");
		}
		rowstart = newBuffer->GetBufferEnd() - row;
	}
	else
	{
		 rowstart = this->GetBufferEnd() - row;
	}

	y = Height;
	while(y--)
	{
		bmpstream->Write(rowstart,1,row);

		if (alig != 0)
		{
			bmpstream->Write(&zero,1,alig);
		}

		rowstart -= row;
	}

	if (newBuffer)
	{
		delete newBuffer;
	}

	if(closestream)
	{
		bmpstream->Close();
	}
}