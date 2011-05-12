#ifndef TBUFFER_H
#define TBUFFER_H

#include "tbytearray.h"
#include "tmemorydriver.h"
#include "texception.h"

#define NFOUND	0xFFFFFFFF

class TBuffer: public TByteArray
{
public:
	dword Index; // Index of current position

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

	inline void Allocate(dword _Capacity)
	{
		this->TByteArray::Allocate(_Capacity);
		Index = 0;
	}

	inline void Use(byte* _Data, dword _Capacity)
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
		if (Capacity == Index)
		{
			Grow(Capacity * 2);
		}
		Data[Index++] = value;
	}

	inline void AddByteNoCheck(byte value)
	{
		Data[Index++] = value;
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
	inline void AddBytesIncreasing(byte* values, dword length)
	{
		if (Capacity - Index < length)
		{
			Grow(Capacity * 2);
		}
		MemoryDriver::Copy(Data+Index,values,length);
		Index += length;
	}

	inline void AddBytes(byte* values,dword length)
	{
		if (Capacity - Index < length)
		{
			throw Exception("Buffer is full");
		}

		MemoryDriver::Copy(Data+Index,values,length);
		Index += length;
	}

	inline void GetBytes(byte* dst, dword length)
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