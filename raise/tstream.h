
#ifndef TSTREAM_H
#define TSTREAM_H

#include "raisetypes.h"
#include "texception.h"

#define FILESTREAM_CLASSID		0


enum SeekOrigin
{
	sBegin = SEEK_SET,
	sCurrent = SEEK_CUR,
	sEnd = SEEK_END
};

class RDLL TStream
{
public:
	// maybe these can be abstract functions.

	//dword	Length;

	virtual ui32	Position() = 0;
	
	virtual ui32	Length()
	{
		throw new Exception("Not Supported");
	}
	
	virtual void	Close() = 0;
	virtual void	Flush() = 0;
	virtual int		Read(void* buffer,int size,int count) = 0;
	virtual int		ReadByte() = 0;
	virtual void	Seek(ui32 offset,SeekOrigin origin) = 0;
	virtual void	Write(const void* buffer,int size,int count) = 0;
	virtual void	WriteByte(byte value) = 0;

	virtual bool	CanRead() = 0;
	virtual bool	CanWrite() = 0;
	virtual bool	CanSeek() = 0;

	/*virtual String&	ReadLine();
	virtual String& ReadToEnd();

	//TODO: put write line codes
	virtual void WriteLine(String& value);*/
};

typedef TStream Stream;


#endif