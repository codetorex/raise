#ifndef TBUFFER_H
#define TBUFFER_H

#include "tmemorydriver.h"
#include "texception.h"

#define NFOUND	0xFFFFFFFF

class TBuffer
{
public:
	byte* Data;
	dword Capacity;
	dword Index; // Index of current position

	TBuffer()
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

	TBuffer(int _capacity)
	{
		Initialize(_capacity);
	}

	void Initialize(dword _cap)
	{
		Data = new byte[_cap];
		Capacity = _cap;
		Index = 0;
	}

	inline void Advance(int offset)
	{
		Index += offset;
	}

	inline void SetPosition(int newpos)
	{
		Index = newpos;
	}

	void AddByte(byte value)
	{
		if (Capacity == Index)
		{
			throw Exception("Buffer is full");
		}
		Data[Index++] = value;
	}

	inline void AddBytes(byte* values,dword length)
	{
		if (Capacity - Index < length)
		{
			throw Exception("Buffer is full");
		}

		memcpy(Data + Index,values,length);
		Index += length;
	}

	inline void GetBytes(byte* dst, dword length)
	{
		if (Capacity - Index < length)
		{
			throw Exception("Buffer is not that big");
		}

		memcpy(dst,Data + Index,length);
	}

	inline int GetAvailable()
	{
		return Capacity - Index;
	}

	byte GetByte()
	{
		if (Capacity == Index)
		{
			throw Exception("Buffer is not that big");
		}

		return Data[Index++]; // -1 because index is already adjusted
	}

	byte PeekByte()
	{
		if (Capacity == Index)
		{
			throw Exception("Buffer is not that big");
		}

		return Data[Index];
	}

	inline void Rewind()
	{
		Index = 0;
	}

	inline void Reset()
	{
		memset(Data,0,Capacity);
		Rewind();
	}

	int Search(const byte* needle,int length)
	{
		if (Index == 0) return NFOUND;
		return MemoryDriver::Search(Data,Index,needle,length);
	}

	int SearchPattern(const byte* needle,const byte* pattern,int length)
	{
		if (Index == 0) return NFOUND;
		return MemoryDriver::SearchPattern(Data,Index,needle,pattern,length);
	}
};

class TVirtualBuffer: public TBuffer
{
public:
	dword VirtualStart;
	dword VirtualEnd;
	dword Length;

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

	inline dword GetVirtualIndex()
	{
		return VirtualStart + Index;
	}

	inline void SetVirtual(dword start,dword end)
	{
		VirtualStart = start;
		VirtualEnd = end;
		Length = end - start;
		//Index = Length;
	}

	inline dword VirtualSearch(const byte* needle,int nlength)
	{
		int found = Search(needle,nlength);
		if (found > -1)
		{
			return VirtualStart + found;
		}
		return NFOUND;
	}

	inline dword VirtualSearchPattern(const byte* needle, const byte* pattern, int nlength)
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