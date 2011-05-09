#include "stdafx.h"
#include "tbitmaptga.h"
#include "tbitmap.h"
#include "tstream.h"
#include "tbinaryreader.h"
#include "mmathdriver.h"
#include "tmemorydriver.h"

// TODO: currently color indexed ones is not working. implement it if needed...


/*
OLD HEADER STUFF
class TGAHeader
{
public:
	byte IDLength; // field 1
	byte ColorMapType; // field 2
	byte ImageType; // field 3

	// field 4
	word ColorMapStart;
	word ColorMapLength;
	byte ColorMapEntrySize;

	// field 5
	short XOrigin;
	short YOrigin;
	word Width;
	word Height;
	byte PixelDepth;
	byte ImageDescriptor;

	byte* ImageID; // field 6, length is IDLength

	byte* ColorMapData; // field 7

	byte* ImageData; // field 8 is where image date comes
};*/

enum TGAOrigin
{
	TO_BottomLeft = 0,
	TO_BottomRight = 1,
	TO_TopLeft = 2,
	TO_TopRight = 3,
};

class TGALoader
{
private:
	TBinaryReader* tga;

public:
		
	int IDLength;
	int ColorMapType;
	int ImageType;

	int ColorMapStart;
	int ColorMapLength;
	int ColorMapEntrySize;
	int ColorMapEntrySizeByte;

	int XOrigin;
	int YOrigin;
	int Width;
	int Height;
	int PixelDepth; // Pixel data length in bits
	int PixelSize; // Pixel data length in bytes
	int AlphaBits;
	int ImageDescriptor;
	TGAOrigin ImageOrigin;

	byte* ImageID;
	byte* ColorMapData;

	int ImageSize;


	TGALoader(TBinaryReader* _tga)
	{
		tga = _tga;
	}

	~TGALoader()
	{
		if (ImageID)
		{
			delete [] ImageID;
		}

		if (ColorMapData)
		{
			delete [] ColorMapData;
		}
	}

	void ReadHeader()
	{
		IDLength = tga->ReadByte();
		ColorMapType = tga->ReadByte();
		ImageType = tga->ReadByte();

		ColorMapStart = tga->ReadWord();
		ColorMapLength = tga->ReadWord();
		ColorMapEntrySize = tga->ReadByte();

		XOrigin = tga->ReadWord();
		YOrigin = tga->ReadWord();
		Width = tga->ReadWord();
		Height = tga->ReadWord();
		PixelDepth = tga->ReadByte();
		ImageDescriptor = tga->ReadByte();

		if (IDLength > 0)
		{
			ImageID = tga->ReadBytes(IDLength);
		}
		else
		{
			ImageID = 0;
		}

		if (ColorMapType == 1)
		{
			ReadColorMap();
		}
		else
		{
			ColorMapData = 0;
		}

		PixelSize = DIV8(PixelDepth); //(int)MMathDriver::Ceil((float)PixelDepth / 8.0f);
		AlphaBits = ImageDescriptor & 0xF;
		ImageOrigin = (TGAOrigin)((ImageDescriptor & 0x30) >> 4);

		ImageSize = Width * Height * (PixelSize);
	}

	inline bool IsColorMapped()
	{
		return (ColorMapType == 1);
	}

	byte* GetColor(int index)
	{
		return ColorMapData + ((index + ColorMapStart) * ColorMapEntrySizeByte);
	}

	void LoadUncompressedImage(TBitmap* bmp)
	{
		// TODO: seems fucked up
		if (AlphaBits == 0 && PixelDepth == 24)
		{
			tga->Read(bmp->Data,1, ImageSize);
		}
		else if(AlphaBits == 8 || PixelDepth == 32)
		{
			// I HOPE THERE IS NO OTHER USAGE OF 32 BIT TGA OTHER THAN ALPHA CHANNEL
			tga->Read(bmp->Data,1, ImageSize);
		}
		else
		{
			throw Exception("Unsupported format");
		}
	}

	void LoadCompressedImage(TBitmap* bmp)
	{
		int currentByte = 0;
		int chunkHeader = 0;

		while(currentByte < ImageSize)
		{
			chunkHeader = tga->ReadByte();

			if (chunkHeader < 128)
			{
				chunkHeader++;
				int dataSize = tga->Read(bmp->Data + currentByte,PixelSize,chunkHeader);

				if ( dataSize == 0)
				{
					throw Exception("TGA doesnt have enough data");
				}

				currentByte += dataSize;
			}
			else
			{
				chunkHeader -= 127;
				int dataOffset = currentByte;
				int dataSize = tga->Read(bmp->Data + currentByte,PixelSize,1);

				if (dataSize == 0 || dataSize != PixelSize)
				{
					throw Exception("TGA doesnt have enough data");
				}

				currentByte += PixelSize;

				currentByte += MemoryDriver::Repeat(bmp->Data + currentByte, bmp->Data + dataOffset,PixelSize,chunkHeader-1); // -1 because we wrote one just before
			}
		}
	}

private:
	void ReadColorMap()
	{
		switch (ColorMapEntrySize)
		{
		case 15:
		case 16:
			ColorMapEntrySizeByte = 2;
			break;

		case 24:
			ColorMapEntrySizeByte = 3;
			break;

		case 32:
			ColorMapEntrySizeByte = 4;
			break;
		}

		int ColorMapTotalLength = ColorMapEntrySizeByte * ColorMapLength;
		ColorMapData = tga->ReadBytes(ColorMapTotalLength);
	}

};


void TBitmapTGA::ReadBitmap( TBitmap* bmp, Stream* src )
{
	TBinaryReader* binaryReader = new TBinaryReader(src);

	// TODO: try reading header?
	TGALoader Loader(binaryReader);
	Loader.ReadHeader();

	bmp->Release();

	bmp->BufferFormat = TBitmapFormats::fBGR;
	if (Loader.IsColorMapped())
	{
		if (Loader.ColorMapEntrySize == 4)
		{
			bmp->BufferFormat = TBitmapFormats::fARGB;
		}
	}
	else
	{
		if (Loader.AlphaBits > 0 || Loader.PixelDepth == 32) 
		{
			// I HOPE THERE IS NO OTHER USAGE OF 32 BIT TGA OTHER THAN ALPHA CHANNEL
			bmp->BufferFormat = TBitmapFormats::fARGB;
		}
	}

	bmp->Create(Loader.Width,Loader.Height,bmp->BufferFormat);

	if (Loader.ImageType == 2) // not compressed RGB
	{
		Loader.LoadUncompressedImage(bmp);
	}
	else if (Loader.ImageType == 10)
	{
		Loader.LoadCompressedImage(bmp);
	}
	else
	{
		throw Exception("Unsupported format");
	}

	// Fix the origin.
	switch(Loader.ImageOrigin)
	{
	case TO_BottomLeft:
		bmp->FlipVertical();
		break;

	case TO_BottomRight:
		bmp->FlipVertical();
		bmp->FlipHorizontal();
		break;

	case TO_TopLeft:
		break;

	case TO_TopRight:
		bmp->FlipHorizontal();
		break;
	}
}

void TBitmapTGA::Install()
{
	TBitmapTGA* tga = new TBitmapTGA();
	Readers.Add(tga);
	Writers.Add(tga);
}

void TBitmapTGA::WriteBitmap( TBitmap* bmp, Stream* dst )
{
	throw Exception("Not Implemented");
}