#ifndef TSTRING_H
#define TSTRING_H

#include "tcharacter.h"
#include "tbytearray.h"
#include "tstringdriver.h"

class TString: private TByteArray
{
private:
	dword ByteLength;

	inline void CountCharsAndBytes()
	{
		StringDriver::LengthUTF8((char*)Data,Length,ByteLength);
	}

public:
	dword Length;

	TString()
	{
		ByteLength = 0;
		Length = 0;
	}

	/**
	 * @brief String literal constructor.
	 * @param value The value.
	 */
	TString(const char* value): TByteArray()
	{
		Data = (byte*)value;
		CountCharsAndBytes();
		Capacity = 0;
	}

	/**
	 * @brief Capacity based constructor for faster operations.
	 * @param _Capacity The capacity.
	 */
	TString(int _Capacity): TByteArray(_Capacity)
	{
		Length = 0;
		ByteLength = 0;
		Data[0] = 0;
	}

	/**
	 * @brief Copy constructor.
	 * @param other The other string.
	 */
	TString( const TString& other)
	{
		Capacity = 0;
		*this = other;
	}

	~TString()
	{
		Free();
	}

	bool Have(ch32 character) const
	{

	}
};

#endif