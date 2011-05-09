#ifndef TENCODER_H
#define TENCODER_H

#include "tcharacter.h"

class TEncoder
{
public:
	virtual void GetByte(byte*& data, ch32 value) = 0;
};

#endif