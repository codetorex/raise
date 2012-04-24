#include "stdafx.h"
#include "tbitmapcodecs.h"

TArray<TBitmapReader*> TBitmapReader::Readers;
TArray<TBitmapWriter*> TBitmapWriter::Writers;

TBitmapPNG TBitmapCodecs::Png;
TBitmapBMP TBitmapCodecs::Bmp;
TBitmapTGA TBitmapCodecs::Tga;
