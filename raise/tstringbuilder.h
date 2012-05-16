#ifndef TSTRINGBUILDER_H
#define TSTRINGBUILDER_H

#include "tstringformat.h"
#include "texceptionlow.h"


class TStringBuilder: protected TByteArray
{
public:
	ui32 Length;
	ui32 ByteLength;

	friend class TString;
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
	TStringBuilder(TString& stringBuffer)
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

	void InitializeCapacity(int capacity)
	{
		if(Data) delete Data;
		this->Allocate(capacity);
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

	inline void UnbindStringBuffer(TString& stringBuffer)
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
		LowLevelNotImplemented(SOURCENAME(4),__LINE__);
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

	inline void Append(const TString& value)
	{
		InternalAppend(value.Data,value.ByteLength,value.Length);
	}

	inline void Append(const TString& fmt, sfp arg0)
	{
		TString result = TString::Format(fmt,arg0);
		Append(result);
	}

	inline void Append(const TString& fmt, sfp arg0, sfp arg1)
	{
		TString result = TString::Format(fmt,arg0,arg1);
		Append(result);
	}

	inline void Append(const TString& fmt, sfp arg0, sfp arg1,sfp arg2)
	{
		TString result = TString::Format(fmt,arg0,arg1,arg2);
		Append(result);
	}

	inline void Append(const TString& fmt, sfp arg0, sfp arg1,sfp arg2,sfp arg3)
	{
		TString result = TString::Format(fmt,arg0,arg1,arg2,arg3);
		Append(result);
	}

	/**
	 * Appends string value left padded.
	 */
	inline void AppendPadded(const TString& value,int padlength, char padchar = ' ', bool truncate = false)
	{
		InternalAppendPadded(value.Data,value.ByteLength,value.Length,padlength,padchar,truncate);
	}

	inline void AppendLine()
	{
		AppendChar('\n');
	}

	inline void AppendLine(const TString& value)
	{
		InternalAppend(value.Data,value.ByteLength,value.Length);
		AppendLine();
	}

	inline void AppendLine(const TString& fmt, sfp arg0)
	{
		TString result = TString::Format(fmt,arg0);
		AppendLine(result);
	}

	inline void AppendLine(const TString& fmt, sfp arg0, sfp arg1)
	{
		TString result = TString::Format(fmt,arg0,arg1);
		AppendLine(result);
	}

	inline void AppendLine(const TString& fmt, sfp arg0, sfp arg1,sfp arg2)
	{
		TString result = TString::Format(fmt,arg0,arg1,arg2);
		AppendLine(result);
	}

	inline void AppendLine(const TString& fmt, sfp arg0, sfp arg1,sfp arg2,sfp arg3)
	{
		TString result = TString::Format(fmt,arg0,arg1,arg2,arg3);
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

	string ToString();

	string ToString(int startIndex, int length);

};

#endif