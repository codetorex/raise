#ifndef TBYTEARRAY_H
#define TBYTEARRAY_H

#include "raisetypes.h"

class TByteArray
{
public:
	byte* Data;
	dword Capacity;

	inline TByteArray()
	{
		Data = 0;
		Capacity = 0;
	}

	inline TByteArray(dword _Capacity)
	{
		Allocate(_Capacity);
	}

	inline TByteArray(byte* _Data, dword _Capacity)
	{
		Use(_Data,_Capacity);
	}

	inline void Use(const TByteArray& value)
	{
		Data = value.Data;
		Capacity = value.Capacity;
	}

	inline void Use(byte* _Data, dword _Capacity)
	{
		Data = _Data;
		Capacity = _Capacity;
	}

	inline void Allocate(dword _Capacity)
	{
		Data = new byte [_Capacity];
		Capacity = _Capacity;
	}
};

#endif