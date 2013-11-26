#ifndef NPACKETREADER_H
#define NPACKETREADER_H

#include "npacket.h"
#include "mdecimal.h"

#define READX(a) 		a r = *(a*)Data;\
						Data += sizeof(a);\
						return r;

class NPacketReader
{
public:
	NPacket* Packet;
	byte* Data;
	byte* Start; // will be used for how much data read

	inline ui32 GetRemainingData()
	{
		ui32 started = Data - Packet->Data;
		ui32 result = Packet->Length - started;
		return result;
	}

	inline ui32 GetReadedDataLength()
	{
		return (Data - Start);
	}

	NPacketReader(NPacket* pck, int index = 0)
	{
		Packet = pck;
		if (index > pck->Length)
		{
			throw Exception("Index out of length");
		}
		Data = Packet->Data +index;
		Start = Data;
	}

	inline byte ReadByte()
	{
		return *(Data++);
	}

	inline byte PeekByte()
	{
		return *(Data);
	}

	inline void ReadByteArray(byte* dst, int length)
	{
		while(length--)
		{
			*(dst++) = *(Data++);
		}
	}

	inline decimal ReadDecimal()
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	inline double ReadDouble()
	{
		READX(double);
	}

	inline i16 ReadInt16()
	{
		READX(i16);
	}

	inline i32 ReadInt32()
	{
		READX(i32);
	}

	inline i64 ReadInt64()
	{
		READX(i64);
	}

	inline ui16 ReadUInt16()
	{
		READX(ui16);
	}

	inline ui32 ReadUInt32()
	{
		READX(ui32);
	}

	inline ui64 ReadUInt64()
	{
		READX(ui64);
	}

	inline float ReadFloat()
	{
		READX(float);
	}

	inline String ReadString()
	{
		ui32 ByteLength = ReadUInt32();
		String r(Data,ByteLength);
		Data += ByteLength;
		return r;
	}
};

#endif