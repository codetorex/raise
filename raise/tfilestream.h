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
	str8		FilePath;
	FILE*		FileHandle;
	FileMode	CurrentMode;

	TFileStream()
	{
		FileHandle = 0;
		CurrentMode = fm_NotOpened;
	}

	static TFileStream* Open(const str8& path, FileMode mode)
	{
		FILE* FileHandle = fopen(path.Chars,FileModeConversion[mode]);
		if (FileHandle == NULL)
		{
			throw Exception("Can't open file");
		}
		return new TFileStream(FileHandle,path,mode);
	}

	TFileStream(FILE* fHandle,const str8& path,FileMode mode)
	{
		FileHandle = fHandle;
		FilePath = path;
		CurrentMode = mode;
	}

	TFileStream(const str8& path, FileMode mode )
	{
		FileHandle = fopen(path.Chars,FileModeConversion[mode]);
		if (FileHandle == NULL)
		{
			throw Exception("Can't open file");
			/*CurrentMode = fm_NotOpened;
			return;*/
		}
		FilePath = path;
		CurrentMode = mode;
	}

	inline dword Position()
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

	inline void Seek(dword offset,SeekOrigin origin)
	{
		fseek(FileHandle,offset,origin);
	}

	inline void Write(void* buffer,int size,int count)
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