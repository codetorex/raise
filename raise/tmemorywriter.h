#ifndef TMEMORYWRITER_H
#define TMEMORYWRITER_H

#include "mdecimal.h"
#include "tqueue.h"

/**
 * I am not totally happy about this name
 * This version doesn't checks for capacity use with care!
 */
class TMemoryWriter
{
public:
	byte* Data; // Current Read PTR
	byte* Start;

	TMemoryWriter()
	{
		InitializeMemoryWriter(0);
	}

	inline void InitializeMemoryWriter(byte* addr, int index)
	{
		InitializeMemoryWriter(addr + index);
	}

	inline void InitializeMemoryWriter(byte* addr)
	{
		Data = addr;
		Start = addr;
	}

	inline ui32 GetWrittenByteCount()
	{
		return (Data - Start);
	}

	inline void WriteByte(byte value)
	{
		*(Data++) = value;
	}

	/**
	 * TODO: use TArray<byte> here.
	 */
	inline void WriteByteArray(byte* value, int length)
	{
		// LOL WTF!
		while (length--)
		{
			*(Data++) = *(value++);
		}
	}

	/**
	 * Decimal not implemented yet. lol
	 */
	inline void WriteDecimal(decimal& value)
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	inline void WriteDouble(double value)
	{
		*(double*)Data = value;
		Data += sizeof(double);
	}

	inline void WriteInt16(i16 value)
	{
		*(i16*)Data = value;
		Data += sizeof(i16);
	}

	inline void WriteInt32(i32 value)
	{
		*(i32*)Data = value;
		Data += sizeof(i32);
	}

	inline void WriteInt64(i64 value)
	{
		*(i64*)Data = value;
		Data += sizeof(i64);
	}

	inline void WriteUInt16(ui16 value)
	{
		*(ui16*)Data = value;
		Data += sizeof(ui16);
	}

	inline void WriteUInt32(ui32 value)
	{
		*(ui32*)Data = value;
		Data += sizeof(ui32);
	}

	inline void WriteUInt64(ui64 value)
	{
		*(ui64*)Data = value;
		Data += sizeof(ui64);
	}

	inline void WriteFloat(float value)
	{
		*(float*)Data = value;
		Data += sizeof(float);
	}

	inline void WriteString(const String& value)
	{
		WriteUInt32(value.ByteLength);
		WriteByteArray(value.Data,value.ByteLength);
	}

	inline void Reset()
	{
		Data = Start;
	}

	/**
	 * Enqueues data to a bytequeue and resets pointers.
	 */
	inline void ToByteQueue(TByteQueue& queue)
	{
		queue.Enqueue(Start, GetWrittenByteCount());
		Reset();
	}
};

#endif