#ifndef TCHARBUFFER_H
#define TCHARBUFFER_H

#include "tbytearray.h"
#include "tcharacter.h"

// Holds charActrs in buffer
class TCharBuffer
{
public:
	TByteArray Buffer;

	ch32* Data;
	dword Capacity;

	dword Index;

	TCharBuffer()
	{
		Data = 0;
		Capacity = 0;
		Index = 0;
	}

	TCharBuffer(dword _Capacity)
	{
		InitializeArray( _Capacity );
	}

	void InitializeArray(dword _Capacity)
	{
		Capacity = _Capacity;
		Buffer.Allocate(_Capacity * sizeof(ch32)); // 1 char holds maximum of 4 bytes
		Data = (ch32*)Buffer.Data;
		Index = 0;
	}

	// Adds a characters without bound checking
	inline void AddCharacterUnsafe( ch32 chr )
	{
		Data[ Index++ ] = chr;
	}

	void Grow( dword _NewCharCapacity )
	{
		Capacity = _NewCharCapacity;
		Buffer.Grow( _NewCharCapacity * sizeof(ch32) );
		Data = (ch32*)Buffer.Data;
	}

	inline void Rewind()
	{
		Index = 0;
	}

	/**
	 * @brief Adds a character to buffer, if buffer is full it grows the capacity
	 */
	inline void AddCharacter( ch32 chr )
	{
		if ( Index == Capacity )
		{
			Grow( Capacity * 2 );
		}

		AddCharacterUnsafe( chr );
	}


};


#endif