#include "stdafx.h"
#include "tbitmapbmp.h"
#include "tbitmap.h"

#ifdef LINUX 

struct BITMAPFILEHEADER 
{
	ui16 bfType;
	ui32 bfSize;
	ui16 bfReserved1;
	ui16 bfReserved2;
	ui32 bfOffBits;
} __attribute((packed));

struct BITMAPINFOHEADER
{
	ui32 biSize;
	int biWidth;
	int biHeight;
	ui16 biPlanes;
	ui16 biBitCount;
	ui32 biCompression;
	ui32 biSizeImage;
	ui32 biXPelsPerMeter;
	ui32 biYPelsPerMeter;
	ui32 biClrUsed;
	ui32 biClrImportant;
}  __attribute((packed));

#endif

void TBitmapBMP::ReadBitmap( TBitmap* bmp, Stream* src )
{
	int y;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	src->Read(&fileHeader,sizeof(BITMAPFILEHEADER),1);
	src->Read(&infoHeader,sizeof(BITMAPINFOHEADER),1);


	if (infoHeader.biCompression != 0 || (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32))
	{
		throw Exception("Unsupported BMP format");
		return;
	}

	bool bottomUp = true;
	if (infoHeader.biHeight < 0)
	{
		bottomUp = false;
		infoHeader.biHeight = abs(infoHeader.biHeight);
	}

	bmp->Release();

	bmp->BufferFormat = BitmapFormats->fBGR;
	if(infoHeader.biBitCount == 32)
	{
		bmp->BufferFormat = BitmapFormats->fRGBA;
	}

	bmp->Create(infoHeader.biWidth,infoHeader.biHeight,bmp->BufferFormat);

	int rowWidth = infoHeader.biWidth * bmp->BufferFormat->BytesPerItem; // row width for file, has padding
	int realRowWidth = rowWidth; // row width for bitmap, doesnt has padding
	if (rowWidth % 4)
	{
		rowWidth += 4 - (rowWidth % 4);
	}
	byte* row = new byte[rowWidth];


	if (bottomUp)
	{
		y = bmp->Height;
		while (y--)
		{
			byte* rowstart = bmp->GetPixel(0,y);
			src->Read(row,1,rowWidth);
			memcpy(rowstart,row,realRowWidth);
		}
	}
	else
	{
		for (y = 0;y<bmp->Height;y++)
		{
			byte* rowstart = bmp->GetPixel(0,y);
			src->Read(row,1,rowWidth);
			memcpy(rowstart,row,realRowWidth);
		}
	}
}

void TBitmapBMP::WriteBitmap( TBitmap* bmp, Stream* dst, TBitmapWriterParameters* params )
{
	int y;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = bmp->PixelCount * 3;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;
	infoHeader.biWidth = bmp->Width;
	infoHeader.biHeight = bmp->Height;

	fileHeader.bfSize = sizeof(BITMAPFILEHEADER);
	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	dst->Write(&fileHeader,1,sizeof(BITMAPFILEHEADER));
	dst->Write(&infoHeader,1,sizeof(BITMAPINFOHEADER));

	ui32 row = bmp->Width * 3;
	ui32 alig = row % 4;
	ui32 zero = 0;

	byte* rowstart;
	TCompositeBuffer* newBuffer = 0;
	if (bmp->BufferFormat != BitmapFormats->fBGR)
	{
		newBuffer = bmp->ConvertCopy(BitmapFormats->fBGR);
		if (newBuffer == NULL)
		{
			throw Exception("No converter for this format");
		}
		rowstart = newBuffer->GetBufferEnd() - row;
	}
	else
	{
		rowstart = bmp->GetBufferEnd() - row;
	}

	y = bmp->Height;
	while(y--)
	{
		dst->Write(rowstart,1,row);

		if (alig != 0)
		{
			dst->Write(&zero,1,alig);
		}

		rowstart -= row;
	}

	if (newBuffer)
	{
		delete newBuffer;
	}
}