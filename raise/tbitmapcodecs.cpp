#include "stdafx.h"
#include "tbitmapcodecs.h"

Array<TBitmapReader*> TBitmapReader::Readers;
Array<TBitmapWriter*> TBitmapWriter::Writers;

TBitmapPNG TBitmapCodecs::Png;
TBitmapBMP TBitmapCodecs::Bmp;
TBitmapTGA TBitmapCodecs::Tga;
