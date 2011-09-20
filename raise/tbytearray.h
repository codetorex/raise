#ifndef TBYTEARRAY_H
#define TBYTEARRAY_H

#include "raisetypes.h"
#include "tmemorydriver.h"

class TByteArray
{
public:
	byte* Data;
	ui32 Capacity;

	/**
	 * @brief Constructor that copies existing byte array to new array with new capacity.
	 * @param [in] other Other byte array that to be copied to new.
	 * @param _NewCapacity New capacity (if smaller than existing capacity will be extended)
	 */
	inline void InitializeByteArray(const TByteArray* other, ui32 _NewCapacity)
	{
		if (_NewCapacity < other->Capacity) _NewCapacity = other->Capacity;
		Data = new byte[_NewCapacity];
		MemoryDriver::Copy(Data,other->Data,other->Capacity);
	}

	inline void InitializeByteArray(ui32 _Capacity)
	{
		Data = 0;
		Allocate(_Capacity);
	}

	inline void InitializeByteArray(byte* _Data, ui32 _Capacity)
	{
		Use(_Data,_Capacity);
	}

	inline void Use(const TByteArray& value)
	{
		Data = value.Data;
		Capacity = value.Capacity;
	}

	inline void Detach()
	{
		Use( 0, 0 );
	}

	inline void Use(byte* _Data, ui32 _Capacity)
	{
		Data = _Data;
		Capacity = _Capacity;
	}

	inline void Grow(ui32 _Capacity)
	{
		if ( !Data ) return Allocate(_Capacity);
		if (_Capacity <= Capacity) return;
		byte* NData = new byte [_Capacity];
		MemoryDriver::Copy(NData,Data,Capacity);
		delete Data;
		Use(NData,_Capacity);
	}

	inline void Grow()
	{
		assert(Capacity != 0);
		Grow(Capacity * 2);
	}

	inline void Allocate(ui32 _Capacity)
	{
		Free();
		Data = new byte [_Capacity];
		Capacity = _Capacity;
	}

	inline void Clear()
	{
		MemoryDriver::Set(Data,0,Capacity);
	}

	inline void Free()
	{
		if ( Data ) delete [] Data;
		Use(0,0);
	}

	~TByteArray()
	{
		Free();
	}

// Constructors
public:

	inline TByteArray()
	{
		Data = 0;
		Capacity = 0;
	}

	inline TByteArray(ui32 _Capacity)
	{
		InitializeByteArray(_Capacity);
	}

	inline TByteArray(const TByteArray* other, ui32 _NewCapacity)
	{
		InitializeByteArray(other,_NewCapacity);
	}
};

class TSharedByteArray: public TByteArray
{
public:
	int RefCount;
	

	TSharedByteArray(): TByteArray()
	{
		RefCount = 1;
	}

	TSharedByteArray(TByteArray* ary)
	{
		Data = ary->Data;
		Capacity = ary->Capacity;
		RefCount = 1;
	}

	TSharedByteArray(byte* _Data, ui32 _Capacity)
	{
		Data = _Data;
		Capacity = _Capacity;
		RefCount = 1;
	}
};

#endif