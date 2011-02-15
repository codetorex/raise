#ifndef TBUFFEREDSTREAM_H
#define TBUFFEREDSTREAM_H

#include "tstream.h"
#include "tbuffer.h"

class TBufferedStream: public TStream
{
public:
	TStream* Source;
	TVirtualBuffer VBuffer;

	bool WriteBuffer; // True = write, false = read

	TBufferedStream(TStream* src, int bufferSize = 1024)
	{
		assert(src!=0);
		Source = src;
		VBuffer.Initialize(1024);
		WriteBuffer = true;
	}
	
	inline dword Position()
	{
		return VBuffer.GetVirtualIndex();
	}

	void Close();
	void Flush();
	int Read(void* buffer,int size,int count);
	int ReadByte();
	void Seek(dword offset,SeekOrigin origin);
	void Write(void* buffer,int size,int count);
	int Peek();

	void WriteByte(byte value)
	{
		throw new Exception("Not implemented");
	}

	inline bool CanRead()
	{
		return Source->CanRead();
	}

	inline bool CanWrite()
	{
		return Source->CanWrite();
	}

	inline bool CanSeek()
	{
		return Source->CanSeek();
	}
};

#endif