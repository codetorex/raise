#ifndef NPACKETWRITER_H
#define NPACKETWRITER_H

#include "npacket.h"
#include "tmemorywriter.h"

/**
 * Advanced and improved version of npacketbuilder, and this will going to replace it?
 * TODO: take Endianness into account!
 * This is something like memory writer? TBinaryWriter? TByteArray?
 * WARNING! You should check available capacity before writing.
 */
class NPacketWriter: public TMemoryWriter
{
public:
	NPacket* Packet;

	NPacketWriter()
	{
		Packet = 0;
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
		InitializeMemoryWriter(pck->Data,index);
	}

	inline void InitializePacketWriter(byte* pData)
	{
		Packet = 0;
		InitializeMemoryWriter(pData);
	}

	inline void Finalize()
	{
		Packet->Length += GetWrittenByteCount();
	}
};

#endif