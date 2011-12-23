#ifndef NPACKET_H
#define NPACKET_H

#include "raisetypes.h"
#include "tbytearray.h"

class NPacket: public TByteArray
{
public:
	/// Holds total bytes to be written or read.
	ui32		TotalBytes;

	/// While writing a packet, this determines how much of it is written
	ui32		SentBytes; 
};

#endif