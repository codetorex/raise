#ifndef TBUFFEREDSTREAM_H
#define TBUFFEREDSTREAM_H

#include "tstream.h"
#include "tbuffer.h"


/**
* A buffer is a block of bytes in memory used to cache data, 
* thereby reducing the number of calls to the operating system. 
* Buffers improve read and write performance. 
* A buffer can be used for either reading or writing, but never both simultaneously. 
*/
class TBufferedStream: public TStream
{
private:
	int ReadNextChunk();
	int ReadBigData(void* buffer, int total);
	int ReadSmallData(void* buffer,int total);

public:
	TStream* Source;
	TVirtualBuffer VBuffer;

	bool WriteBuffer; // True = write, false = read

	TBufferedStream(TStream* src, int bufferSize = 4096)
	{
		assert(src!=0);
		Source = src;
		if (bufferSize == -1)
		{
			int srclength = src->Length();
			VBuffer.Allocate(srclength);
			VBuffer.SetVirtual(0,srclength);
			Source->Read(VBuffer.Data,1,srclength);
		}
		else
		{
			VBuffer.Allocate(bufferSize);	
		}

		WriteBuffer = true;
	}
	
	inline dword Position()
	{
		return VBuffer.GetVirtualIndex();
	}

	/**
	* Closes the stream and commits suicide.
	*/
	void Close();
	void Flush();
	inline int Read(void* buffer,int size,int count)
	{
		dword totalSize = size * count;
		if (totalSize <= VBuffer.Capacity)
		{
			return ReadSmallData(buffer,totalSize);
		}
		return ReadBigData(buffer,totalSize);
	}

	//int OldRead(void* buffer,int size,int count);

	/**
	* Trys to read a byte, throws exception if end of stream.
	*/
	int ReadByte();
	void Seek(dword offset,SeekOrigin origin);
	void Write(void* buffer,int size,int count);
	int PeekByte();

	void WriteByte(byte value);

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