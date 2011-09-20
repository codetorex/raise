#ifndef TFILESTREAM_H
#define TFILESTREAM_H

#include "stdafx.h"
#include "tstring.h"
#include "tstream.h"


enum FileMode
{
	fm_Read,
	fm_ReadWrite,
	fm_Write,
	fm_WriteRead,
	fm_Append,
	fm_AppendRead,
	fm_NotOpened,
};

static const char* FileModeConversion[] = { "rb", "r+b", "wb", "w+b", "ab", "a+b" };


class RDLL TFileStream: public TStream
{
public:
	TString		FilePath;
	FILE*		FileHandle;
	FileMode	CurrentMode;

	TFileStream()
	{
		FileHandle = 0;
		CurrentMode = fm_NotOpened;
	}

/*	static TFileStream* Open(const TString& path, FileMode mode)
	{
		FILE* FileHandle = fopen(path.Chars,FileModeConversion[mode]);
		if (FileHandle == NULL)
		{
			throw Exception("Can't open file");
		}
		return new TFileStream(FileHandle,path,mode);
	}

	static TFileStream* OpenRead(const TString& path)
	{

	}*/

	TFileStream(FILE* fHandle,const TString& path,FileMode mode)
	{
		FileHandle = fHandle;
		FilePath = path;
		CurrentMode = mode;
	}

	TFileStream(const TString& path, FileMode mode )
	{
		//TODO: use better string usage like OpenFile( converttowide( path ) )
		int result = fopen_s(&FileHandle,(char*)path.Data,FileModeConversion[mode]);
		if (result != 0)
		{
			throw Exception("Can't open file");
			/*CurrentMode = fm_NotOpened;
			return;*/
		}
		FilePath = path;
		CurrentMode = mode;
	}

	inline ui32 Position()
	{
		return ftell(FileHandle);
	}

	/**
	* Closes the stream and commits suicide.
	*/
	inline void Close()
	{
		fclose(FileHandle);
		delete this; // NOTE: this guy added later if problems occur when commiting sucide, revert back to old system which is deletes these stream objects by in caller functions.
	}

	inline void Flush()
	{
		fflush(FileHandle);
	}

	inline int Read(void* buffer, int size, int count)
	{
		return fread(buffer,size,count,FileHandle);
	}

	inline int ReadByte()
	{
		byte tmp;
		Read(&tmp,1,1);
		return (int)tmp;
	}

	inline void Seek(ui32 offset,SeekOrigin origin)
	{
		fseek(FileHandle,offset,origin);
	}

	inline void Write(const void* buffer,int size,int count)
	{
		fwrite(buffer,size,count,FileHandle);
	}

	inline void WriteByte(byte value)
	{
		Write(&value,1,1);
	}

	inline bool CanRead()
	{
		if (FileHandle == NULL || CurrentMode == fm_NotOpened)
		{
			return false;
		}
		if (CurrentMode == fm_Write ||CurrentMode == fm_Append)
		{
			return false;
		}
		return true;
	}

	inline bool CanWrite()
	{
		if (FileHandle == NULL || CurrentMode == fm_NotOpened)
		{
			return false;
		}
		if (CurrentMode == fm_Read)
		{
			return false;
		}
		return true;
	}

	inline bool CanSeek()
	{
		if (FileHandle == NULL || CurrentMode == fm_NotOpened )
		{
			return false;
		}
		if (CurrentMode == fm_Append ||CurrentMode == fm_AppendRead)
		{
			return false;
		}
		return true;
	}
};

#endif