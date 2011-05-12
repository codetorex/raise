#ifndef TSTRINGDRIVER_H
#define TSTRINGDRIVER_H

#include "tcharacter.h"

class StringDriver
{
public:
	inline static void LengthASCII(char* data, dword& length, dword& bytelength)
	{
		length = bytelength = strlen(data);
	}



	/**
	 * @brief Calculates length of a utf-8 encoded string.
	 * @param [in] data pointer to string.
	 * @param [out] length The character length of string.
	 * @param [out] bytelength The byte length of string.
	 */
	static void LengthUTF8(char* data, dword& length, dword& bytelength)
	{

	}

	/**
	 * @brief Encodes a character in utf-8.
	 * @param value The character.
	 * @param [in,out] data data pointer (should have enough space).
	 * @return Bytes written.
	 */
	inline static int EncodeUTF8(ch32 value, byte*& data)
	{

	}


	/**
	 * @brief Encodes a character in utf-8 with checking capacity.
	 * @param value The character.
	 * @param [in,out] data [in,out] data pointer.
	 * @param cap The capacity of data pointer.
	 */
	inline static void EncodeUTF8(ch32 value, byte*& data, int cap)
	{

	}


	/**
	 * @brief Decode a utf-8 character.
	 * @param [in,out] data [in,out] pointer reference to data.
	 * @return The character.
	 */
	inline static ch32 DecodeUTF8(byte*& data)
	{

	}

	inline static ch32 GetNextUTF8(char*& data)
	{
		ch32 result;
		if (*data > 0)
		{
			result = *data;
			data++;
		}
		else
		{
			return DecodeUTF8(data);
		}
		return result;
	}
};

#endif

/*
11101010 10111111 10111111 00000001
11000000 11000000 11000000 11000000     and
---------------------------------------------
11000000 10000000 10000000 00000000
01000000 01000000 01000000 01000000     xor
----------------------------------------------
10000000 11000000 11000000 01000000
11000000 11000000 11000000 11000000     xor
----------------------------------------------
01000000 00000000 00000000 10000000
01000000 01000000 01000000 01000000     and
----------------------------------------------
10000000 01000000 01000000 11000000
10000000 10000000 10000000 10000000     and
----------------------------------------------
10000000 00000000 00000000 10000000
00000001 00000000 00000000 00000001



11110000  11110000 11110000 11110000
11111111  11111111 11111111 11111111     and
---------------------------------------------
11110000  11110000 11110000 11110000
*/