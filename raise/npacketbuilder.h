#ifndef NPACKETBUILDER_H
#define NPACKETBUILDER_H

#include "npacket.h"
#include "tstring.h"
#include "tbuffer.h"
#include "tconvert.h"

class NPacketBuilder: public TBuffer
{
private:



	inline void InternalAppend(const void* src, int length)
	{
		CheckCapacity(length);
		MemoryDriver::Copy(Data + Index, src,length);
		Index+=length;
	}

public:

	NPacketBuilder() : TBuffer(512) 
	{
		Index = 0;
	}

	/**
	 * Creates string builder that uses specified memory region.
	 * Can be used for stack space but don't forget to call UnbindByteArray.
	 * And keep stack enough capacity or it will try to grow it so destroy the stack.
	 */
	NPacketBuilder(byte* dstPtr, int dstSize)
	{
		Data = dstPtr;
		Capacity = dstSize;
		Index = 0;
	}

	NPacketBuilder(int _capacity) : TBuffer(_capacity) 
	{
		Index = 0;
	}

	/*NPacketBuilder( NPacket* _packet )
	{
		Packet = _packet;
	}*/

	/**
	 * Converts int value to string and appends it.
	 */
	inline void AppendString(int value)
	{
		char tmp[16];
		int ln = TConvert::ToCharArrayInt(value,tmp,16);
		InternalAppend(tmp,ln);
	}

	/**
	 * Appends an utf-8 string to packet.
	 */
	inline void AppendString(const TString& value)
	{
		InternalAppend(value.Data,value.ByteLength);
	}

	inline void AppendLine(const TString& value)
	{
		InternalAppend(value.Data,value.ByteLength+1);
		Data[Index-1] = '\n';
	}

	inline void AppendLine()
	{
		CheckCapacity(1);
		Data[Index++] = '\n';
	}

	inline NPacket* ToPacket()
	{
		// TODO: USE A OBJECT POOL HERE
		NPacket* result = new NPacket();
		
		result->Use(Data,Capacity);
		result->TotalBytes = Index;
		result->SentBytes = 0;
		Detach();

		return result;
	}
};

#endif