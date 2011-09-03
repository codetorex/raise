#ifndef TSTRINGBUILDER_H
#define TSTRINGBUILDER_H

#include "tstring.h"
#include "tconvert.h"


class TStringBuilder: private TByteArray
{
private:
	dword Length;
	dword ByteLength;

	inline void CheckCapacity(dword length)
	{
		if ( ByteLength + length >= Capacity )
		{
			Grow(Capacity * 2);
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
		}
	}

	inline void InternalAppendPadded(const void* src,int srclength, int charlength, int padcount, char padchar = ' ', bool truncate = true )
	{
		if (charlength >= padcount)
		{
			if (truncate)
			{
				if (srclength == charlength)
				{
					InternalAppend(src,padcount,padcount);
				}
				else
				{
					int blen = StringDriver::Count((byte*)src,padcount) - (byte*)src;
					InternalAppend(src,blen,padcount);
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
				padcount = -padcount;
				InternalAppend(src,srclength,charlength);
				InternalRepeatChar(padchar,padcount - charlength);
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

	TStringBuilder(int capacity): TByteArray(capacity)
	{
		Length = 0;
		ByteLength = 0;
	}

	~TStringBuilder()
	{
		delete [] Data;
	}


	/**
	 * Should be called if used stack array. So it wont get tried to be destroyed.
	 */
	inline void UnbindByteArray()
	{
		Data = 0;
	}

	inline void Append(int value)
	{
		char tmp[16];
		int ln = TConvert::ToCharArray(value,tmp,16);
		InternalAppend(tmp,ln,ln);
	}

	inline void AppendPadded(int value, int padlength, char padchar = '0', bool truncate = false)
	{
		char tmp[16];
		int ln = TConvert::ToCharArray(value,tmp,16);
		InternalAppendPadded(tmp,ln,ln,padlength,padchar,truncate);
	}

	inline void AppendChar(char c)
	{
		CheckCapacity(1);
		Data[ByteLength++] = c;
	}

	inline void Append(const TString& value)
	{
		InternalAppend(value.Data,value.ByteLength,value.Length);
	}

	inline void AppendLine()
	{
		AppendChar('\n');
	}

	string ToString();

	string ToString(int startIndex, int length);

};

#endif