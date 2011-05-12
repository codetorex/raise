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
		Data = 0;
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

	inline void Grow(dword _Capacity)
	{
		if ( !Data ) return Allocate(_Capacity);
		if (_Capacity <= Capacity) return;
		byte* NData = new byte [_Capacity];
		MemoryDriver::Copy(NData,Data,Capacity);
		delete Data;
		Use(NData,_Capacity);
	}

	inline void Allocate(dword _Capacity)
	{
		if (Data) delete Data;
		Data = new byte [_Capacity];
		Capacity = _Capacity;
	}

	inline void Clear()
	{
		MemoryDriver::Set(Data,0,Capacity);
	}

	inline void Free()
	{
		if ( Data ) delete Data;
		Use(0,0);
	}
};

#endif