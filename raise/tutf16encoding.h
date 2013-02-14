#ifndef TUTF16ENCODING_H
#define TUTF16ENCODING_H

#include "tencoding.h"
#include "texception.h"
#include "tstring.h"

/**
 * @brief Interface for UTF-8 encoding.
 */
class UTF16Encoding: public TEncoding
{
public:
	UTF16Encoding()
	{
		Name = "utf-16";
		DisplayName = "Unicode (UTF-16)";
		MinLengthPerChar = 16;
		MaxLengthPerChar = 32;
		CodePage = 1200;
	}

	inline int GetMaxByteCount(int charCount) const
	{
		return charCount * 4;
	}

	inline int GetMaxCharCount(int byteCount) const
	{
		return byteCount / 4;
	}

	inline bool IsSurrogate(ui32 curchar) const
	{
		return curchar >= 0xD800 && curchar <= 0xDFFF;
	}

	inline int GetByteCount(const ch32* data,int length) const
	{
		int result = 0;
		while(length--)
		{
			ch16 curchar = *data++;
			if (!IsSurrogate(curchar))
			{
				result+=2;
			}
			else
			{
				result+=4;
			}
		}
		
		return result;
	}

	inline int GetCharCount(const byte* data, int capacity) const
	{
		ui32 leng,byteleng;
		Length(data,capacity,leng,byteleng);
		return leng;
	}

	inline void Length(const byte* data, ui32 capacity, ui32& length, ui32& bytelength) const
	{

		ui32 llength = 0;
		ui32 lblength = 0;

		while(lblength < capacity)
		{
			ch16 curchar = *(ch16*)&data[lblength];
			if (!IsSurrogate(curchar) )
			{
				llength++;
				lblength += 2;
			}
			else
			{
				llength++;
				lblength += 4;
			}
		}

		length = llength;
		bytelength = lblength;
	}

	ch32 Decode(ch16 hi,ch16 lo) const
	{
		ui32 X = (hi & ((1 << 6) -1)) << 10 | lo & ((1 << 10) -1);
		ui32 W = (hi >> 6) & ((1 << 5) - 1);
		ui32 U = W + 1;
		ui32 C = U << 16 | X;
		return C;
	}

	inline ch32 GetChar(const byte* src,int& byteLength) const
	{
		ch16 hi = *(ch16*)src;
		if (!IsSurrogate(hi))
		{
			byteLength= 2;
			return (ch32)hi;
		}
		
		byteLength = 4;
		ch16 lo = *(ch16*)(src+2);

		return Decode(hi,lo);
	}

	TString GetString(const ch16* src) const
	{
		int ln;
		TString result(64);
		byte* bsrc = (byte*)src;
		ch32 curChar = GetChar(bsrc,ln);
		bsrc += ln;
		while ( curChar > 0 ) 
		{
			result.AppendUnicode(curChar);
			curChar = GetChar(bsrc,ln);
			bsrc += ln;
		} 

		return result;
	}


	/**
	 * @brief Converts TString to ch16* destination.
	 * @param src Source string
	 * @param dst wide character destination
	 * @param dstcapacity byte length of destination
	 */
	void ConvertString(const TString& src,byte* dst, int dstcapacity) const
	{
		ch16 tmp[4];

		TCharacterEnumerator schars(src);
		while(schars.MoveNext())
		{
			int ln = GetBytes((byte*)tmp,schars.Current) / 2;
			if (dstcapacity < ln)
			{
				return;
			}
			switch (ln)
			{
			case 2:
				*(ch16*)dst = tmp[0];
				dst += ln;
			}

			dstcapacity -= ln;
		}
		*(ch16*)dst = 0;
	}

	inline int GetBytes(byte* dst, ch32 character) const
	{
		if (!IsSurrogate(character))
		{
			*(ch16*)dst = (ch16)character;
			return 2;
		}
		
		throw NotImplementedException();
		/*

		UTF16 X = (UTF16) C;
		UTF32 U = (C >> 16) & ((1 << 5) - 1);
		UTF16 W = (UTF16) U - 1;
		UTF16 HiSurrogate = HI_SURROGATE_START | (W << 6) | X >> 10;


		const UTF16 LO_SURROGATE_START = 0xDC00
		UTF16 X = (UTF16) C;
		UTF16 LoSurrogate = (UTF16) (LO_SURROGATE_START | X & ((1 << 10) - 1));


		*/


		/*if (character < 0x7F)
		{
			*dst = (byte)character;
			return 1;
		}
		return StringDriver::Encode(dst,character);*/
	}

	int GetChars( byte* bytes, int bytecount, ch32* chars , int charcapacity ) const
	{
		return 0;
		//throw NotImplementedException();
	}

/*	TCharBuffer GetChars( byte* bytes, int bytecount )
	{
		throw NotImplementedException();
	}*/

	int GetBytes( ch32* chars, int charcount, byte* bytes, int bytecapacity ) const
	{
		throw NotImplementedException();
	}

/*	TByteBuffer GetBytes ( ch32* chars, int charcount )
	{
		throw NotImplementedException();
	}*/

	ui32 GetChars( byte* bytes, ui32 bytecount, TCharBuffer& buffer ) const
	{
		throw NotImplementedException();
	}

	TString GetString(byte* bytes, int bytecount) const
	{
		int ln;
		TString result(64);
		ch32 curChar = 0;
		byte* src = bytes;
		int rcount = bytecount;
		while(rcount--)
		{
			ch32 curChar = GetChar(src,ln);
			src += ln;
			result += curChar;
		}
		return result;
	}

	/*int GetChars( byte* bytes, int bytecount, TCharBuffer& buffer)
	{
		throw NotImplementedException();
	}*/
};

#endif