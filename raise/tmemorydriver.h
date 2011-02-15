#ifndef TMEMORYDRIVER_H
#define TMEMORYDRIVER_H

#include "raisetypes.h"


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

	inline static int SearchWord(const byte* src, int srcLength, word needle)
	{
		if (srcLength <= 1) return -1;
		int i = srcLength - 1;
		while(i--)
		{
			if (*(word*)(&src[i]) == needle)
			{
				return i;
			}
		}
		return -1;
	}

	inline static int SearchDWord(const byte* src, int srcLength, dword needle )
	{
		if (srcLength <= 3) return -1;
		int i = srcLength - 3;

		while(i--)
		{
			if (*(dword*)(&src[i]) == needle)
			{
				return i;
			}
		}
		return -1;
	}

	inline static int SearchVariable(const byte* src, const int srcLength, const byte* needle,int length)
	{
		int i = (srcLength+1) - length;
		if (i <= 0) return -1;

		const byte* rptr = src;
		while(i--)
		{
			if (*rptr == *needle)
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

	inline static int Search(const byte* src, const int srcLength, const byte* needle,int length)
	{
		assert(srcLength > 0);
		assert(length > 0);

		switch (length)
		{
		case 1:
			return SearchByte(src,srcLength,*needle);

		case 2:
			return SearchWord(src,srcLength,*(word*)needle);

		case 4:
			return SearchDWord(src,srcLength,*(dword*)needle);

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
};

#endif