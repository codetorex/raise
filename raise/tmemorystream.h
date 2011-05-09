#ifndef TMEMORYSTREAM_H
#define TMEMORYSTREAM_H

#include "tfile.h"
#include "tbuffer.h"

// TODO: writing should increase capacity.


class TMemoryStream: public TStream
{
private:
	void LoadStream(TStream* sourceStream)
	{
		DataBuffer.Allocate(sourceStream->Length());
		sourceStream->Read(DataBuffer.Data,1,DataBuffer.Capacity);
		sourceStream->Close();
		EndOfStream = false;
	}

public:
	TBuffer DataBuffer;
	bool EndOfStream;

	inline TMemoryStream(byte* data, int capacity)
	{
		DataBuffer.Use(data,capacity);
	}

	inline TMemoryStream(const TByteArray& byteArray)
	{
		DataBuffer.Use(byteArray.Data,byteArray.Capacity);
	}

	TMemoryStream(TBuffer* data)
	{
		DataBuffer.Data = data->Data;
		DataBuffer.Capacity = data->Capacity;
	}

	TMemoryStream(const str8& filePath)
	{
		LoadStream(File::OpenRead(filePath));
	}

	TMemoryStream(TStream* source)
	{
		LoadStream(source);
	}

	inline dword Position()
	{
		return DataBuffer.Index;
	}

	inline dword Length()
	{
		return DataBuffer.Capacity;
	}

	inline void Close()
	{
		delete this;
	}

	inline void Flush()
	{

	}

	inline int Read(void* buffer,int size,int count)
	{
		int TotalLength = size * count;

		if (DataBuffer.Index + TotalLength > DataBuffer.Capacity)
		{
			TotalLength = DataBuffer.Capacity - DataBuffer.Index;
		}

		if (TotalLength <= 0)
		{
			EndOfStream = true;
			return 0;
		}

		MemoryDriver::Copy(buffer,DataBuffer.Data + DataBuffer.Index,TotalLength);
		return TotalLength;
	}

	inline int ReadByte()
	{
		return DataBuffer.ReadByte();
	}

	inline int PeekByte()
	{
		return DataBuffer.PeekByte();
	}

	inline void Seek(dword offset,SeekOrigin origin)
	{
		switch(origin)
		{
		case sBegin:
			DataBuffer.Set(offset);
			break;

		case sCurrent:
			DataBuffer.Advance(offset);
			break;

		case sEnd:
			DataBuffer.Set(DataBuffer.Capacity - offset);
			break;
		}

		EndOfStream = false;

		if ( DataBuffer.Index >= DataBuffer.Capacity)
		{
			EndOfStream = true;
			DataBuffer.Index = DataBuffer.Capacity - 1;
		}

		if (DataBuffer.Index < 0)
		{
			DataBuffer.Index = 0;
		}
	}

	inline void Write(void* buffer,int size,int count)
	{
		int TotalLength = size * count;
		DataBuffer.AddBytesIncreasing((byte*)buffer,TotalLength);
	}

	inline void WriteByte(byte value)
	{
		DataBuffer.AddByteIncreasing(value);
	}

	inline bool CanRead()
	{
		return true;
	}

	inline bool CanWrite()
	{
		return true;
	}

	inline bool CanSeek()
	{
		return true;
	}
};

#endif