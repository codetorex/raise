#ifndef TSTRINGDRIVER_H
#define TSTRINGDRIVER_H

#include "tcharacter.h"
#include "tarray.h"

const static byte bytesFromUTF8[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 0, 0
};

class StringDriver
{
public:

	const static Array<ch32> Whitespaces;

	/**
	 * @brief Decodes a character from UTF-8 string.
	 * @param [in] charptr Pointer to character.
	 * @param [in,out] byteLength Decoded byte count.
	 * @return Decoded character.
	 */
	static inline ch32 Decode(const byte* charptr, int& byteLength)
	{
		if ((*charptr) < 0x80)
		{
			byteLength = 1;
			return (ch32)*charptr;
		}
		return DecodeOnlyUnicode(charptr, byteLength);
	}

	static ch32 DecodeOnlyUnicode(const byte* charptr, int& byteLength);

	static inline bool IsASCII(ch32 c)
	{
		return (c < 0x80 );
	}

	/**
	 * @brief Encodes one character to target byte array without safety.
	 * @param [out] charptr Pointer to byte array where encoded character to be written.
	 * @param value The character to be encoded.
	 * @return Written byte count.
	 */
	static inline int Encode(byte* charptr, ch32 c)
	{
		if (c < 0x80)
		{
			*charptr = (char)c;
			return 1;
		}
		return EncodeOnlyUnicode(charptr,c);
	}
	
	/**
	 * Skips checking character if its ASCII, it just encodes unicode characters and fails if ASCII.
	 */
	static int EncodeOnlyUnicode(byte* charptr, ch32 c);

	inline static ch32 DecodeFast(const byte* charptr, int& byteLength)
	{
		if (*charptr < 0x80)
		{
			byteLength = 1;
			return *charptr;
		}
		
		return Decode(charptr,byteLength);
	}

	inline static ch32 DecodeAdv( byte*& src )
	{
		if (*src < 0x80)
		{ 
			return *src++;
		}
		int ln;
		ch32 result = DecodeOnlyUnicode(src,ln);
		src += ln;
		return result;
	}

	inline static void EncodeAdv( byte*& dst, ch32 character)
	{
		if (character < 0x80)
		{
			*dst = (byte)character;
			dst++;
			return;
		}
		int ln = StringDriver::EncodeOnlyUnicode(dst,character);
		dst += ln;
	}

	static void Length(const byte* data,int capacity, ui32& length, ui32& bytelength)
	{
		length = 0;
		bytelength = 0;
		for (int i=0;i<capacity;i++)
		{
			if (data[i] == 0) break;
			i += bytesFromUTF8[ data[i] ];
			length++;
		}
	}

	inline static bool IsTrail(byte c)
	{
		return ( c & 0xc0 ) == 0x80;
	}

	/**
	 * @brief Reversed decoding for scanning from reverse.
	 * @param [in,out] charptr Pointer to character array.
	 * @param length Should be remaining length of array for avoiding any catastrophic event.
	 * @param [out] bytelength The byte length of read character in array.
	 * @return The character.
	 */
	inline static ch32 ReverseDecode(byte* charptr, int length, int& bytelength)
	{
		if (*charptr < 0x80)
		{
			bytelength = 1;
			return (ch32)*charptr;
		}
		
		while(length--)
		{
			if( IsTrail( *(--charptr) ) )
			{
				continue;
			}
			else
			{
				return Decode(charptr,bytelength);
			}
		}

		return 0;
	}

	/**
	 * Counts chars in a byte array.
	 */
	static ui32 Length(const byte* data, ui32 capacity)
	{
		ui32 result = 0;
		for (ui32 i=0;i<capacity;i++)
		{
			if (!IsTrail(data[i]))
			{
				result++;
			}
		}
		return result;
	}

	static void Length(const byte* data, ui32& length, ui32& bytelength)
	{
		length = 0;
		bytelength = 0;
		int i=0;
		byte c = data[i];
		while( c != 0 )
		{
			if (!IsTrail(c))
			{
				length++;
			}
			bytelength++;
			c = *(++data);
		}
	}


	/**
	 * @brief Counts chars and returns pointer when counter ends.
	 * @param src The source.
	 * @param charToCount Number of character to count.
	 * @return Address of where counter stops.
	 */
	static byte* Count(byte* src, int charToCount)
	{
		if (charToCount == 0) return src;

		byte curbyte = *src;
		while(curbyte)
		{
			if (!IsTrail(curbyte))
			{
				if (charToCount == 0)
				{
					return src;
				}
				charToCount--;
			}
			curbyte = *(++src);
		}

		return src;
	}

	static int RequiredBytes(const ch32* data,int length);

	static inline int GetMaxByteCount( int charCount )
	{
		return charCount * 4;
	}

	static inline int GetMaxCharCount( int byteCount )
	{
		return byteCount;
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