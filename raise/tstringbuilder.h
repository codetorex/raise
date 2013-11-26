#ifndef TSTRINGBUILDER_H
#define TSTRINGBUILDER_H

#include "raisetypes.h"
#include "tbytearray.h"
#include "tstringformat.h"


class TStringBuilder: protected TByteArray
{
public:
	ui32 Length;
	ui32 ByteLength;

	friend class String;
	friend class TStringWriter;

protected:
	inline void AppendCharFaster(char c)
	{
		Data[ByteLength++] = c;
	}

	inline void ReCount()
	{
		StringDriver::Length(Data,Length,ByteLength);
	}

	inline void CheckCapacity(ui32 length)
	{
		if ( ByteLength + length >= Capacity )
		{
			Grow(((ByteLength + length) * 2));
		}
	}

	inline void InternalAppend(const void* src, int srclength, int charlength)
	{
		CheckCapacity(srclength);
		MemoryDriver::Copy(Data + ByteLength, src,srclength);
		ByteLength += srclength;
		Length += charlength;
	}

	inline void InternalRepeatChar(char chr,int cnt)
	{
		CheckCapacity(cnt);
		while(cnt--)
		{
			Data[ByteLength++] = chr;
			Length++;
		}
	}

	inline void InternalAppendPadded(const void* src,int srclength, int charlength, int padcount, char padchar = ' ', bool truncate = true )
	{
		int abspad = padcount < 0 ? -padcount : padcount;
		if (charlength >= abspad)
		{
			if (truncate)
			{
				if (srclength == charlength)
				{
					InternalAppend(src,abspad,abspad);
				}
				else
				{
					int blen = StringDriver::Count((byte*)src,abspad) - (byte*)src;
					InternalAppend(src,blen,abspad);
				}
			}
			else
			{
				InternalAppend(src,srclength,charlength);
			}
		}
		else
		{
			bool padleft = true;
			if (padcount < 0) // left aligned text
			{
				InternalAppend(src,srclength,charlength);
				InternalRepeatChar(padchar,abspad - charlength);
			}
			else // right aligned text
			{
				InternalRepeatChar(padchar, padcount - charlength);
				InternalAppend(src,srclength,charlength);
			}
		}
	}

public:


	TStringBuilder() : TByteArray(512) 
	{
		Length = 0;
		ByteLength = 0;
	}

	/**
	 * Creates string builder that uses specified memory region.
	 * Can be used for stack space but don't forget to call UnbindByteArray.
	 * And keep stack enough capacity or it will try to grow it so destroy the stack.
	 */
	TStringBuilder(byte* dstPtr, int dstSize)
	{
		Data = dstPtr;
		Capacity = dstSize;
		Length = 0;
		ByteLength = 0;
	}

	/**
	 * Uses string as a buffer.
	 */
	TStringBuilder(String& stringBuffer)
	{
		Data = stringBuffer.Data;
		Capacity = stringBuffer.Capacity;
		Length = stringBuffer.Length;
		ByteLength = stringBuffer.ByteLength;
	}

	TStringBuilder(int capacity): TByteArray(capacity)
	{
		Length = 0;
		ByteLength = 0;
	}

	void InitializeCapacity(ui32 capacity)
	{
		if (capacity > this->Capacity)
		{
			if(Data) delete Data;
			this->Allocate(capacity);
		}
		
		Length = 0;
		ByteLength = 0;
	}

	inline const byte* GetData() const
	{
		return Data;
	}

	/**
	 * Should be called if used stack array. So it wont get tried to be destroyed.
	 */
	inline void UnbindByteArray()
	{
		Data = 0;
	}

	inline void UnbindStringBuffer(String& stringBuffer)
	{
		Data = 0;
		stringBuffer.Length = Length;
		stringBuffer.ByteLength = ByteLength;
	}

	/**
	 * Removes characters from end of the string builder.
	 */
	inline void RemoveLast(int count)
	{
		if (Length == ByteLength)
		{
			Length -= count;
			ByteLength -= count;
			Data[ByteLength] = 0;
			return;
		}
		throw NotImplementedException(__FILE__,__LINE__);
	}

	inline void Append(int value)
	{
		char tmp[16];
		int ln = TConvert::ToCharArrayInt(value,tmp,16);
		InternalAppend(tmp,ln,ln);
	}

	inline void Append(const TStringFormatElementBase& value)
	{
		if (value.PadWidth == 0)
		{
			InternalAppend(value.Source,value.ByteLength,value.CharLength);
		}
		else
		{
			InternalAppendPadded(value.Source,value.ByteLength,value.CharLength,value.PadWidth,value.PadChar);
		}
	}

	/**
	 * Appends int value with left padded.
	 */
	inline void AppendPadded(int value, int padlength, char padchar = '0', bool truncate = false)
	{
		char tmp[16];
		int ln = TConvert::ToCharArrayInt(value,tmp,16);
		InternalAppendPadded(tmp,ln,ln,padlength,padchar,truncate);
	}

	inline void AppendCharFast(char c)
	{
		Data[ByteLength++] = c;
		Length++;
	}

	inline void AppendChar(char c)
	{
		CheckCapacity(1);
		Data[ByteLength++] = c;
		Length++;
	}

	inline void AppendUnicode( ch32 c )
	{
		if ( StringDriver::IsASCII(c) )
		{
			AppendChar(c);
			return;
		}

		byte tmp[8];
		int ln = StringDriver::Encode(tmp,c);
		InternalAppend(tmp,ln,1); // already detaches
	}

	inline void AppendChar(char c, int repeatCount)
	{
		if (repeatCount == 0) return;
		InternalRepeatChar(c,repeatCount);
	}

	inline void Append(const String& value)
	{
		InternalAppend(value.Data,value.ByteLength,value.Length);
	}

	inline void Append(const String& fmt, sfp arg0)
	{
		String result = String::Format(fmt,arg0);
		Append(result);
	}

	inline void Append(const String& fmt, sfp arg0, sfp arg1)
	{
		String result = String::Format(fmt,arg0,arg1);
		Append(result);
	}

	inline void Append(const String& fmt, sfp arg0, sfp arg1,sfp arg2)
	{
		String result = String::Format(fmt,arg0,arg1,arg2);
		Append(result);
	}

	inline void Append(const String& fmt, sfp arg0, sfp arg1,sfp arg2,sfp arg3)
	{
		String result = String::Format(fmt,arg0,arg1,arg2,arg3);
		Append(result);
	}

	/**
	 * Replaces given character with other one
	 * returns Replaced char count
	 */
	inline int ReplaceChar(ch32 matchChar, ch32 replaceChar)
	{
		int count = 0;

		if (IsASCII())
		{
			char mc = matchChar;
			char rc = replaceChar;
			/// TODO: replace this function with faster equivalent?
			for (ui32 i=0;i<ByteLength;i++)
			{
				if (Data[i] == mc)
				{
					Data[i] = rc;
					count++;
				}
			}
		}
		else
		{
			throw NotImplementedException(__FILE__,__LINE__);
		}

		return count;
	}

	/**
	 * Appends string value left padded.
	 */
	inline void AppendPadded(const String& value,int padlength, char padchar = ' ', bool truncate = false)
	{
		InternalAppendPadded(value.Data,value.ByteLength,value.Length,padlength,padchar,truncate);
	}

	/**
	 * Appends stringBUilder value left padded.
	 */
	inline void AppendPadded(const TStringBuilder& value,int padlength, char padchar = ' ', bool truncate = false)
	{
		InternalAppendPadded(value.Data,value.ByteLength,value.Length,padlength,padchar,truncate);
	}

	inline void AppendLine()
	{
		AppendChar('\n');
	}

	inline void AppendLine(const String& value)
	{
		InternalAppend(value.Data,value.ByteLength,value.Length);
		AppendLine();
	}

	inline void AppendLine(const String& fmt, sfp arg0)
	{
		String result = String::Format(fmt,arg0);
		AppendLine(result);
	}

	inline void AppendLine(const String& fmt, sfp arg0, sfp arg1)
	{
		String result = String::Format(fmt,arg0,arg1);
		AppendLine(result);
	}

	inline void AppendLine(const String& fmt, sfp arg0, sfp arg1,sfp arg2)
	{
		String result = String::Format(fmt,arg0,arg1,arg2);
		AppendLine(result);
	}

	inline void AppendLine(const String& fmt, sfp arg0, sfp arg1,sfp arg2,sfp arg3)
	{
		String result = String::Format(fmt,arg0,arg1,arg2,arg3);
		AppendLine(result);
	}

	inline void Clear()
	{
		ByteLength = 0;
		Length = 0;
	}

	inline void PokeZero()
	{
		Data[ByteLength] = 0;
	}

	inline bool IsASCII()
	{
		return (ByteLength == Length);
	}

	void ToLower()
	{
		if (!IsASCII())
		{
			// call ToLowerUnicode
			throw NotImplementedException(__FILE__,__LINE__);
			return;
		}

		for (ui32 i=0;i<ByteLength;i++)
		{
			TChar ch = Data[i];
			Data[i] = ch.ToLower();
		}
	}

	void ToUpper()
	{
		if (!IsASCII())
		{
			// call ToLowerUnicode
			throw NotImplementedException(__FILE__,__LINE__);
			return;
		}

		for (ui32 i=0;i<ByteLength;i++)
		{
			TChar ch = Data[i];
			Data[i] = ch.ToUpper();
		}
	}

	String ToString();

	String ToString(int startIndex, int length);

};

template <int K>
class TStringBuilderStack: public TStringBuilder
{
public:
	byte Temp[K];

	inline TStringBuilderStack(): TStringBuilder(Temp,K)
	{

	}

	inline ~TStringBuilderStack()
	{
		if (Data == Temp)
		{
			UnbindByteArray();
		}
	}
};

#endif