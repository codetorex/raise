#include "stdafx.h"
#include "tfile.h"
#include "mmathdriver.h"
#include "tbuffer.h"

ui32 TFile::ReadAllBytes( const String& path, void* target, ui32 capacity )
{
	if (capacity == 0) return 0;

	TFileStream* fs = new TFileStream(path,fm_Read);
	ui32 readlength = Math.Min(capacity,fs->Length());

	ui32 readed = fs->Read(target,1,readlength);

	fs->Close();

	return readed;
}

ui32 TFile::ReadAllBytes( const String& path, TBuffer& buffer )
{
	TFileStream* fs = new TFileStream(path,fm_Read);
	
	int filelength = fs->Length();
	int bufferavailable = buffer.GetAvailable();

	if (bufferavailable < filelength)
	{
		buffer.Grow((filelength - bufferavailable) + 128);
	}

	ui32 readed = fs->Read(buffer.Data+ buffer.Index,1,filelength);

	buffer.Index += filelength;

	fs->Close();
	return readed;
}