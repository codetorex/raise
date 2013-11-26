#ifndef TBUFFER_H
#define TBUFFER_H

#include "tbytearray.h"
#include "tmemorydriver.h"
#include "tstream.h"

#define NFOUND	0xFFFFFFFF
;
class TBuffer: public TByteArray
{
public:
	ui32 Index; // Index of current position

	inline void CheckCapacity(int length)
	{
		if ( Index + length >= Capacity )
		{
			Grow(Capacity * 2);
		}
	}

	inline TBuffer()
	{
		Index = 0;
		Data = 0;
		Capacity = 0;
	}

	~TBuffer()
	{
		if (Data)
		{
			delete [] Data;
		}
		Data = 0;
		Capacity = 0;
		Index = 0;
	}
	
	inline TBuffer(int _Capacity)
	{
		Allocate(_Capacity);
	}

	inline void Allocate(ui32 _Capacity) // THIS IS VERY WRONG
	{
		this->TByteArray::Allocate(_Capacity);
		Index = 0;
	}

	inline void Use(byte* _Data, ui32 _Capacity)
	{
		this->TByteArray::Use(_Data,_Capacity);
		Index = 0;
	}

	inline void Advance(int offset)
	{
		Index += offset;
	}

	inline void Set(int newpos)
	{
		Index = newpos;
	}

	/**
	* Adds byte to buffer and increases buffer when needed.
	*/
	inline void AddByteIncreasing(byte value)
	{
		CheckCapacity(1);
		Data[Index++] = value;
	}
	
	inline void AddByteNoCheck(byte value)
	{
		Data[Index++] = value;
	}

	inline void AddStreamContent( TStream* stream, bool closeStream = true )
	{
		ui32 streamLength = stream->Length();
		CheckCapacity(streamLength);

		int readedBytes = stream->Read(Data+Index,1,streamLength);
		Index += readedBytes;

		if (closeStream)
		{
			stream->Close();
		}
	}

	/**
	* Adds byte to buffer and throws exception when buffer is full.
	*/
	inline void AddByte(byte value)
	{
		if (Capacity == Index)
		{
			throw Exception("Buffer is full");
		}
		Data[Index++] = value;
	}

	/**
	* Add bytes to buffer and increases buffer when needed.
	*/
	inline void AddBytesIncreasing(byte* values, ui32 length)
	{
		CheckCapacity(length);
		MemoryDriver::Copy(Data+Index,values,length);
		Index += length;
	}

	inline void AddBytes(byte* values,ui32 length)
	{
		if (Capacity - Index < length)
		{
			throw Exception("Buffer is full");
		}

		MemoryDriver::Copy(Data+Index,values,length);
		Index += length;
	}

	inline void GetBytes(byte* dst, ui32 length)
	{
		if (Capacity - Index < length)
		{
			throw Exception("Buffer is not that big");
		}

		MemoryDriver::Copy(dst,Data + Index,length);
		Index += length;
	}

	inline int GetAvailable()
	{
		return Capacity - Index;
	}

	inline int ReadByteNoCheck()
	{
		return Data[Index++];
	}

	inline int ReadByte()
	{
		if (Capacity == Index)
		{
			return -1;
		}

		return Data[Index++]; // -1 because index is already adjusted
	}

	inline int PeekByte()
	{
		if (Capacity == Index)
		{
			return -1;
		}

		return Data[Index];
	}

	inline int PeekByteNoCheck()
	{
		return Data[Index];
	}

	inline void Rewind()
	{
		Index = 0;
	}

	inline void Reset()
	{
		MemoryDriver::Set(Data,0,Capacity);
		Rewind();
	}

	inline int Search(const byte* needle,int length)
	{
		if (Index == 0) return NFOUND;
		return MemoryDriver::Search(Data,Index,needle,length);
	}

	inline int SearchPattern(const byte* needle,const byte* pattern,int length)
	{
		if (Index == 0) return NFOUND;
		return MemoryDriver::SearchPattern(Data,Index,needle,pattern,length);
	}
};

typedef TBuffer TByteBuffer;

class TVirtualBuffer: public TBuffer
{
public:
	ui32 VirtualStart;
	ui32 VirtualEnd;
	ui32 Length;

	TVirtualBuffer()
	{
		VirtualStart = 0;
		VirtualEnd = 0;
		Length = 0;
	}

	TVirtualBuffer(int _cap): TBuffer(_cap)
	{
		VirtualStart = 0;
		VirtualEnd = 0;
		Length = 0;
	}

	inline void VirtualRewind()
	{
		Rewind();
		VirtualStart = 0xFFFFFFFF;
		VirtualEnd = 0xFFFFFFFF;
		Length = 0;
	}

	inline ui32 GetVirtualIndex()
	{
		return VirtualStart + Index;
	}

	inline void SetVirtual(ui32 start,ui32 end)
	{
		VirtualStart = start;
		VirtualEnd = end;
		Length = end - start;
		//Index = Length;
	}

	inline ui32 VirtualSearch(const byte* needle,int nlength)
	{
		int found = Search(needle,nlength);
		if (found > -1)
		{
			return VirtualStart + found;
		}
		return NFOUND;
	}

	inline ui32 VirtualSearchPattern(const byte* needle, const byte* pattern, int nlength)
	{
		int found = SearchPattern(needle,pattern,nlength);
		if(found > -1)
		{
			return VirtualStart + found;
		}
		return NFOUND;
	}
};

#endif