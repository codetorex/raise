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

	inline int GetMaxByteCount(int charCount) const
	{
		return StringDriver::GetMaxByteCount(charCount);
	}

	inline int GetMaxCharCount(int byteCount) const
	{
		return StringDriver::GetMaxCharCount(byteCount);
	}


	inline int GetByteCount(const ch32* data,int length) const
	{
		return StringDriver::RequiredBytes(data,length);
	}

	inline int GetCharCount(const byte* data, int capacity) const
	{
		ui32 leng,byteleng;
		Length(data,capacity,leng,byteleng);
		return leng;
	}
	
	inline void Length(const byte* data, ui32 capacity, ui32& length, ui32& bytelength) const
	{
		StringDriver::Length(data,capacity,length,bytelength);
	}

	inline ch32 GetChar(const byte* src,int& byteLength) const
	{
		return StringDriver::Decode(src,byteLength);
	}

	inline int GetBytes(byte* dst, ch32 character) const
	{
		return StringDriver::Encode(dst,character);
	}

	int GetChars( byte* bytes, int bytecount, ch32* chars , int charcapacity ) const
	{
		ch32* cfirst = chars;
		while( bytecount > 0 && charcapacity-- )
		{
			byte curbyte = *bytes;
			if (curbyte < 0x7F)
			{
				*chars++ = (ch32)curbyte;
				bytecount--;
				bytes++;
			}
			else
			{
				int blen = 0;
				*chars++ = StringDriver::Decode(bytes,blen);
				bytes += blen;
				bytecount -= blen;
			}
		}
		return (chars - cfirst) / sizeof(ch32);
	}

	/*TCharBuffer GetChars( byte* bytes, int bytecount )
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}*/

	int GetBytes( ch32* chars, int charcount, byte* bytes, int bytecapacity ) const
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	/*TByteBuffer GetBytes ( ch32* chars, int charcount )
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}*/

	ui32 GetChars( byte* bytes, ui32 bytecount, TCharBuffer& buffer ) const
	{
		ui32 bcstart = bytecount; // storage of bytecount while begining algorithm

		while( bytecount > 0 )
		{
			byte curbyte = *bytes;

			if (curbyte <= 0x7F)
			{
				buffer.AddCharacter( (ch32)curbyte );
				bytecount--;
				bytes++;
			}
			else
			{
				int blen = 0;
				blen = bytesFromUTF8[curbyte];
				if ((int)bytecount < blen)
				{
					return bcstart - bytecount;
				}
				
				buffer.AddCharacter( StringDriver::Decode(bytes,blen) );
				bytes += blen;
				bytecount -= blen;
			}
		}

		return bcstart;
	}

	String GetString(byte* bytes, int bytecount) const
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}
};

#endif