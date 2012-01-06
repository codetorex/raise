#ifndef NPACKET_H
#define NPACKET_H

#include "raisetypes.h"
#include "tbytearray.h"

/**
 * Simple structure for holding data pointer capacity and length.
 */
class NPacket
{
public:
	/// Pointer to where data held
	byte*		Data;

	/// Capacity of Data ptr
	ui32		Capacity;

	/// Holds total bytes to be written or read.
	ui32		Length;

	inline void Allocate(ui32 _Capacity)
	{
		Data = new byte[_Capacity];
		Capacity = _Capacity;
		Length = 0;
	}
	
	inline void Use(byte* _Data,ui32 _Capacity)
	{
		Data = _Data;
		Capacity = _Capacity;
		Length = 0;
	}
};

#endif