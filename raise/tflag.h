#ifndef TFLAG_H
#define TFLAG_H

#include "raisetypes.h"

class TFlag32
{
public:
	ui32 Data;

	inline TFlag32(ui32 _data)
	{
		Data = _data;
	}

	inline TFlag32()
	{
		Data = 0;
	}

	inline void Set(ui32 val)
	{
		Data |= val;
	}

	inline void Unset(ui32 val)
	{
		Data &= ~val;
	}

	inline bool Get(ui32 val) const
	{
		return ((Data & val) != 0);
	}

	inline TFlag32& operator += ( ui32 val )
	{
		Set(val);
		return *this;
	}

	inline TFlag32& operator -= ( ui32 val )
	{
		Unset(val);
		return *this;
	}

	inline bool operator == ( ui32 val ) const
	{
		return Get(val);
	}

	inline TFlag32& operator = ( ui32 val )
	{
		Data = val;
		return *this;
	}

	/*operator ui32& (void)
	{
		return Data;
	}*/
};

typedef TFlag32 flag32;

#endif