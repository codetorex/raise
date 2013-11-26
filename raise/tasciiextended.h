#ifndef TASCIIEXTENDED_H
#define TASCIIEXTENDED_H

#include "tencoding.h"
#include "mmathdriver.h"

/**
 * 32 bit simple cache that represents enough data for extension to be encoded in utf8.
 */
struct UTF8Mapping 
{
	byte Length;
	byte UTF8Data[3];
};

class TASCIIExtendedEncoding: public TEncoding
{
public:
	/// Character mapping
	ch32 Mapping[256];

	/// Simple cache for creating strings out of this encoding. First byte is length.
	UTF8Mapping EncodedMapping[256];

	// TODO: implement index class so we can implement reverse encoding

	TASCIIExtendedEncoding()
	{
		/*Name = "ascii"; // this names should be changed
		DisplayName = "ascii extended";*/
		MinLengthPerChar = 8;
		MaxLengthPerChar = 8;
		//CodePage = 0;
	}

	void UpdateUTF8Encoding()
	{
		for (int i=0;i<256;i++)
		{
			ch32 cur32 = Mapping[i];
			UTF8Mapping& cur8 = EncodedMapping[i];

			cur8.Length = StringDriver::Encode(cur8.UTF8Data,cur32);
		}
	}

	inline int GetMaxByteCount(int charCount) const
	{
		return charCount;
	}

	inline int GetMaxCharCount(int byteCount) const
	{
		return byteCount;
	}

	inline int GetByteCount(const ch32* src,int length) const
	{
		return length;
	}

	inline int GetCharCount(const byte* src, int capacity) const
	{
		return capacity;
	}

	inline void Length(const byte* src,ui32 capacity, ui32& length, ui32& bytelength) const
	{
		length = capacity;
		bytelength = capacity;
	}

	inline ch32 GetChar(const byte* src,int& byteLength) const
	{
		byteLength = 1;
		return Mapping[*src];
	}

	inline ch32 GetCharAdv( byte*& src ) const
	{
		return Mapping[ *src++ ];
	}

	inline int GetBytes(byte* dst, ch32 character) const
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	inline void GetBytesAdv(byte*& dst, ch32 character) const
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	inline ui32 GetChars( byte* bytes, ui32 bytecount, TCharBuffer& buffer ) const
	{
		if (buffer.Capacity < bytecount)
		{
			buffer.Grow(bytecount);
		}

		while (bytecount--)
		{
			buffer.AddCharacterUnsafe( Mapping[*bytes++] );
		}
		return bytecount;
	}

	inline int GetChars(byte* bytes, int bytecount, ch32* chars , int charcapacity) const
	{
		int toread = Math.Min(bytecount,charcapacity);
		int remaining = toread;
		while(remaining--)
		{
			*(chars++) = Mapping[*bytes++];
		}
		return toread;
	}

	inline int GetBytes(ch32* chars, int charcount, byte* bytes, int bytecapacity) const
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	String GetString(byte* bytes, int bytecount) const
	{
		String result(bytecount*3);
		while(bytecount--)
		{
			UTF8Mapping curchar = EncodedMapping[*bytes++];
			int i = 0;
			switch (curchar.Length)
			{
			case 3:
				result.Data[result.ByteLength++] = curchar.UTF8Data[i++];
			case 2:
				result.Data[result.ByteLength++] = curchar.UTF8Data[i++];
			case 1:
				result.Data[result.ByteLength++] = curchar.UTF8Data[i++];
			}
		}
		return result;
	}
};


#endif