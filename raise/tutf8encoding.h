#ifndef TUTF8ENCODING_H
#define TUTF8ENCODING_H

#include "tencoding.h"
#include "tstringdriver.h"


/**
 * @brief Interface for UTF-8 encoding.
 */
class UTF8Encoding: public TEncoding
{
public:
	UTF8Encoding()
	{
		Name = "utf-8";
		DisplayName = "Unicode (UTF-8)";
		MinLengthPerChar = 8;
		MaxLengthPerChar = 32;
		CodePage = 65001;
	}

	// FUNCTIONS ARE ALREADY DEFINED IN STRINGDRIVER BECAUSE INTERNAL USE
	// SO WE WILL JUST MIRROR THOSE FUNCTIONS IN HERE TO COMPLETE THE INTERFACE

	inline int GetMaxByteCount(int charCount)
	{
		return StringDriver::GetMaxByteCount(charCount);
	}

	inline int GetMaxCharCount(int byteCount)
	{
		return StringDriver::GetMaxCharCount(byteCount);
	}


	inline int GetByteCount(const ch32* data,int length)
	{
		return StringDriver::RequiredBytes(data,length);
	}

	inline int GetCharCount(const byte* data, int capacity)
	{
		dword leng,byteleng;
		Length(data,capacity,leng,byteleng);
		return leng;
	}
	
	inline void Length(const byte* data, int capacity, dword& length, dword& bytelength)
	{
		StringDriver::Length(data,capacity,length,bytelength);
	}

	inline ch32 GetChar(const byte* src,int& byteLength)
	{
		if (*src < 0x7F){ byteLength = 1; return *src; }
		return StringDriver::Decode(src,byteLength);
	}

	inline int GetBytes(byte* dst, ch32 character)
	{
		if (character < 0x7F)
		{
			*dst = (byte)character;
			return 1;
		}
		return StringDriver::Encode(dst,character);
	}
};

#endif