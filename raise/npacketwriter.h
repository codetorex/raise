#ifndef NPACKETWRITER_H
#define NPACKETWRITER_H

#include "npacket.h"
#include "texception.h"
#include "mdecimal.h"

/**
 * Advanced and improved version of npacketbuilder, and this will going to replace it?
 * TODO: take Endianness into account!
 * This is something like memory writer? TBinaryWriter? TByteArray?
 * WARNING! You should check available capacity before writing.
 */
class NPacketWriter
{
public:
	NPacket* Packet;
	byte* Data; // Current Read PTR
	byte* Start;

	NPacketWriter()
	{
		Packet = 0;
		Data = 0;
		Start = 0;
	}

	NPacketWriter(NPacket* pck,int index = 0)
	{
		InitializePacketWriter(pck,index);
	}

	inline void InitializePacketWriter(NPacket* pck,int index = 0)
	{
		Packet = pck;
		if (index > pck->Capacity)
		{
			throw Exception("Index out of bounds");
		}
		Data = Packet->Data + index;
		Start = Data;
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
		throw NotImplementedException();
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

	inline void WriteString(const TString& value)
	{
		WriteUInt32(value.ByteLength);
		WriteByteArray(value.Data,value.ByteLength);
	}

	inline void Finalize()
	{
		Packet->Length += GetWrittenByteCount();
	}
};

#endif