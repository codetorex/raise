#include "stdafx.h"
#include "tbitmappng.h"
#include "tbitmap.h"
#include "tstream.h"


#ifndef NOPNGLIB


#include "png.h"
#include "pngstruct.h"

#define PNG_BYTES_TO_CHECK 4


void StreamRead(png_structp png_ptr,png_bytep data,png_size_t length)
{
	TStream* stream = (TStream*)png_ptr->io_ptr;
	stream->Read(data,1,length);
}

void StreamWrite(png_structp png_ptr, png_bytep data, png_size_t length)
{
	TStream* stream = (TStream*)png_ptr->io_ptr;
	stream->Write(data,1,length);
}

void StreamFlush(png_structp png_ptr)
{
	TStream* stream = (TStream*)png_ptr->io_ptr;
	stream->Flush();
}



void TBitmapPNG::ReadBitmap( TBitmap* bmp, Stream* src )
{
	png_byte pbSig[PNG_BYTES_TO_CHECK];
	src->Read(pbSig,1,PNG_BYTES_TO_CHECK);
	if (!png_check_sig(pbSig,PNG_BYTES_TO_CHECK))
	{
		throw Exception("Invalid PNG");
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,(png_error_ptr)NULL, (png_error_ptr)NULL);
	if (!png_ptr)
	{
		throw Exception("Invalid PNG Version");
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr,(png_infopp)NULL, (png_infopp)NULL);
		throw Exception("Memory error");
	}

	png_set_read_fn(png_ptr, (png_voidp)src, StreamRead);

	/*png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
	png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
	return -4;
	}*/

	png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);

	png_read_info(png_ptr, info_ptr);

	ui32 piWidth,piHeight;
	int iBitDepth,iColorType;
	png_get_IHDR(png_ptr, info_ptr, &piWidth, &piHeight, &iBitDepth,&iColorType, NULL, NULL, NULL);

	if (iBitDepth == 16)									png_set_strip_16(png_ptr);
	if (iColorType == PNG_COLOR_TYPE_PALETTE)				png_set_expand(png_ptr);
	if (iBitDepth < 8 )										png_set_expand(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))	png_set_expand(png_ptr);
	if (iColorType == PNG_COLOR_TYPE_GRAY || iColorType == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);

	double dGamma;
	// if required set gamma conversion
	if (png_get_gAMA(png_ptr, info_ptr, &dGamma)) png_set_gamma(png_ptr, (double) 2.2, dGamma);	

	png_read_update_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &piWidth, &piHeight, &iBitDepth,&iColorType, NULL, NULL, NULL);

	long width,height,bit_depth,color_type,channels,rowbytes;
	bool alpha = false;

	width = png_get_image_width(png_ptr,info_ptr);
	height = png_get_image_height(png_ptr,info_ptr);
	bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	color_type = png_get_color_type(png_ptr,info_ptr);
	channels = png_get_channels(png_ptr, info_ptr);
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	if (channels < 3)
	{
		throw Exception("PNG Missing channel");
	}

	if (color_type == PNG_COLOR_TYPE_RGB)
	{
		alpha = false;
	}
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		alpha = true;
	}

	bmp->Create(piWidth,piHeight,alpha? BitmapFormats->fRGBA : BitmapFormats->fRGB);


	png_bytep* potr;
	potr = new png_bytep [piHeight];


	for (ui32 i = 0; i < piHeight; i++)
		potr[i] = bmp->Data + i * rowbytes;

	png_read_image(png_ptr, potr);

	png_read_end(png_ptr, NULL);

	delete [] potr;
}

void TBitmapPNG::WriteBitmap( TBitmap* bmp, Stream* dst, TBitmapWriterParameters* params )
{
	int ciBitDepth = 8;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_byte **ppbRowPointers = NULL;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,(png_error_ptr)NULL, (png_error_ptr)NULL);
	if (!png_ptr)
	{
		throw Exception("Memory error");
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) 
	{
		png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
		throw Exception("Memory error");
	}

	try
	{
		png_set_write_fn(png_ptr, (png_voidp)dst, StreamWrite, StreamFlush);


		int colortype; //= bmp->BufferFormat == tbit & RL_ALPHA ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB;

		if (bmp->BufferFormat == BitmapFormats->fRGBA)
		{
			colortype = PNG_COLOR_TYPE_RGB_ALPHA;
		}
		else if(bmp->BufferFormat == BitmapFormats->fRGB)
		{
			colortype = PNG_COLOR_TYPE_RGB;
		}
		else
		{
			throw Exception("TODO: write code for converting format");
		}


		png_set_IHDR(png_ptr, info_ptr, bmp->Width, bmp->Height, ciBitDepth,colortype, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,PNG_FILTER_TYPE_BASE);
		png_write_info(png_ptr, info_ptr);

		int rowbytes = bmp->Width * bmp->BufferFormat->BytesPerItem;
		ppbRowPointers = new png_byte* [bmp->Height];

		for (int i=0;i<bmp->Height;i++)
		{
			ppbRowPointers[i] = bmp->Data + (i*rowbytes);
		}

		png_write_image (png_ptr, ppbRowPointers);
		png_write_end(png_ptr, info_ptr);
		delete [] ppbRowPointers;
		ppbRowPointers = NULL;

		png_destroy_write_struct(&png_ptr, (png_infopp) NULL);

	}
	catch(void* ms)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp) NULL);

		if(ppbRowPointers)
			delete [] ppbRowPointers;
		throw ObjectException("Writing bmp failed",ms);
	}
}

#endif