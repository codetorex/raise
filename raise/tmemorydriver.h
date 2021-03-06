#ifndef TMEMORYDRIVER_H
#define TMEMORYDRIVER_H

#include "raisetypes.h"

#ifdef LINUX
#include <string.h>
#endif

class MemoryDriver
{
public:
	inline static int Compare(const void* src,const void* dst, int length)
	{
		return memcmp(src,dst,length);
	}

	inline static void Set(void* dst, int value, int length)
	{
		memset(dst,value,length);
	}

	inline static void Copy(void* dst, const void* src, int length)
	{
		memcpy(dst,src,length);
	}

#ifndef LINUX
#ifndef M64
	inline static void ShortCopy(void* _dst,const void* _src,int _size)
	{
		_asm
		{
			mov esi,_src
			mov edi,_dst
			mov ecx,_size
			rep movsb
		}
	}

#else

	inline static void ShortCopy(void* _dst,const void* _src,int _size)
	{q
		_asm
		{
			mov rsi,_src
			mov rdi,_dst
			mov rcx,_size
			rep movsb
		}
	}
#endif

#else
	inline static void ShortCopy(void* _dst,const void* _src,int _size)
	{
		memcpy(_dst,_src,_size); // TODO: fix this
		/*asm
			(
			"cld;"
			"rep\n"
			"movsb;"
			:
		: "S"(_src),"D"(_dst),"c"(_size)
			);*/
	}
#endif

	/**
	* Repeats a data in memory that fits 4 bytes per item.
	* @param dst destination memory
	* @param src source memory
	* @param count how much times source repeated
	* @return written bytes
	*/
	inline static int Repeat4Byte(void* dst, const void* src, int count)
	{
		ui32 nsrc = *(ui32*)src;
		ui32* ndst = (ui32*)dst;
		while(count--)
		{
			*(ndst++) = nsrc;
		}
		return (int)((byte*)ndst - (byte*)dst);
	}

	/**
	* Repeats a data in memory.
	* @param dst destination memory
	* @param src source memory
	* @param length source length
	* @param count how much times source repeated
	* @return written bytes
	*/
	inline static int Repeat(void* dst, const void* src,int length, int count)
	{
		if (length == 4)
		{
			return Repeat4Byte((ui32*)dst,(ui32*)src,count);
		}

		// TODO: maybe asm one can be written?
		byte* ndst = (byte*)dst;
		byte* nsrc = (byte*)src;
		while(count--)
		{
			for (int i=0;i<length;i++)
			{
				ndst[i] = nsrc[i];
			}
			ndst += length;
		}
		return (int)(ndst - (byte*)dst);
	}

	/**
	* Exchanges data between two buffers.
	* @param dst destination
	* @param src source
	* @param length length of data to be exchanged
	* @param returns 0 when successful
	*/
	inline static int Exchange(void* dst, void* src, int length)
	{
		// TODO: optimize this function by utilizing dword copying
		byte tmp;
		byte* row1 = (byte*)dst;
		byte* row2 = (byte*)src;

		while(length--)
		{
			tmp = *row1;
			*row1 = *row2;
			*row2 = tmp;
			row1++;
			row2++;
		}
		return length;
	}

	inline static void Exchange4Byte(void* dst, void* src, int count)
	{
		// TODO: test this algorithm?
		ui32* dsti = (ui32*)dst;
		ui32* srci = (ui32*)src;

		while(count--)
		{
			ui32 tmp = *dsti;
			*dsti = *srci;
			*srci = tmp;
			dsti++;
			srci++;
		}
	}

	inline static int ComparePattern(const void* src, const void* dst, const void* pattern, int length)
	{
		while(length--)
		{
			if (((byte*)pattern)[length] == 'x')
			{
				if (((byte*)src)[length] != ((byte*)dst)[length])
				{
					return 1;
				}
			}
		}
		return 0;
	}

	inline static int SearchByte(const byte* src, int srcLength, byte needle)
	{
		if (srcLength == 0) return -1;
		int i = srcLength;
		while(i--)
		{
			if (src[i] == needle)
			{
				return i;
			}
		}
		return -1;
	}

	inline static int SearchWord(const byte* src, int srcLength, ui16 needle)
	{
		if (srcLength <= 1) return -1;
		int i = srcLength - 1;
		while(i--)
		{
			if (*(ui16*)(&src[i]) == needle)
			{
				return i;
			}
		}
		return -1;
	}

	inline static int SearchDWord(const byte* src, int srcLength, ui32 needle )
	{
		if (srcLength <= 3) return -1;
		int i = srcLength - 3;

		while(i--)
		{
			if (*(ui32*)(&src[i]) == needle)
			{
				return i;
			}
		}
		return -1;
	}

	inline static int SearchVariable(const byte* src, const int srcLength, const void* needle,int length)
	{
		int i = (srcLength+1) - length;
		if (i <= 0) return -1;

		const byte* rptr = src;
		while(i--)
		{
			if (*rptr == *(byte*)needle)
			{
				if (Compare(rptr,needle,length) == 0)
				{
					return (rptr - src);
				}
			}
			rptr++;
		}

		return -1;
	}

	/**
	 * Use this for searching a float number
	 */
	inline static int Search(const byte* src, int srcLength, float needle, float epsilon)
	{
		if (srcLength < sizeof(float))
		{
			return -1;
		}
		
		int i = (srcLength+1) - sizeof(float);

		const byte* rptr = src;
		while (i--)
		{
			float diff =  *(float*)rptr - needle;
			if (diff < epsilon && diff > -epsilon )
			{
				return rptr	- src;
			}
			rptr++;
		}
		return -1;
	}
	
	inline static int Search(const byte* src, const int srcLength, const void* needle,int length)
	{
		assert(srcLength > 0);
		assert(length > 0);

		switch (length)
		{
		case 1:
			return SearchByte(src,srcLength,*(byte*)needle);

		case 2:
			return SearchWord(src,srcLength,*(ui16*)needle);

		case 4:
			return SearchDWord(src,srcLength,*(ui32*)needle);

		default:
			return SearchVariable(src,srcLength,needle,length);
		}
	}

	inline static int SearchPattern(const byte* src, int srcLength, const byte* needle,const byte* pattern,int length)
	{
		assert(srcLength > 0);
		assert(length > 0);

		int i = (srcLength+1) - length;
		if (i <= 0) return -1;

		const byte* rptr = src;
		while(i--)
		{
			if (*rptr == *needle)
			{
				if (MemoryDriver::ComparePattern(rptr,needle,pattern,length) == 0)
				{
					return (rptr - src);
				}
			}
			rptr++;
		}
		return -1;
	}

	/**
	 * Picks data from larger one with help of a pattern
	 * Returns -1 when output capacity is reached
	 * Returns 0 or positive integer of how much bytes written to output
	 */
	static int ReadPattern(const byte* src, int srcLength, const byte* pattern, int patternLength, byte patternMatch, byte* output, int outputCapacity)
	{
		assert(outputCapacity > 0);
		assert(srcLength > 0);
		assert(patternLength > 0);

		int outIndex = 0;
		int dataLength = srcLength < patternLength ? srcLength : patternLength;

		for (int i=0;i<dataLength;i++)
		{
			if (pattern[i] == patternMatch)
			{
				if (outIndex == outputCapacity)
				{
					return -1; // -1 means capacity reached
				}

				output[outIndex] = src[i];
				outIndex++;
			}
		}

		return outIndex;
	}
};

#endif