#ifndef TSTRINGDRIVERFIXEDWIDTH_H
#define TSTRINGDRIVERFIXEDWIDTH_H

// THIS SYSTEM IS OBSOLOTE
// WILL BE REMOVED AFTER TSTRING IS REIMPLEMENTED AS UTF8

#include "tcharacter.h"
#include <string.h>

#ifdef WIN32
#pragma warning( disable: 4996 )

#define _vsnprintf vsnprintf
#define _strlwr strlwr
#define _strupr strupr
#include <string.h>
#include <ctype.h>
#endif

#ifdef LINUX
// with -fshort-wchar compiler option wchar_t will use 16 bits
#include <string>
#include <wchar.h>
#include <stdarg.h>
#include <wctype.h>
#include <memory>

#define _vsnwprintf		vswprintf
#define vsprintf_s		vsnprintf
#define strnset			memset
#define wcsnset			wmemset
inline char* strlwr(char* str) { char* it = str; while (*it != 0) { *it = tolower(*it); ++it; } return str; }
inline char* strupr(char* str) { char* it = str; while (*it != 0) { *it = toupper(*it); ++it; } return str; }
inline wchar_t* wcslwr(wchar_t* str) { wchar_t* it = str; while (*it != 0) { *it = towlower(*it); ++it; } return str; }
inline wchar_t* wcsupr(wchar_t* str) { wchar_t* it = str; while (*it != 0) { *it = towupper(*it); ++it; } return str; }
#endif



class StringDriverFixedWidth
{
public:

	inline static ch16* GetWhitespaces(ch16* k = 0)
	{
		return L"\x20\x09\x0A\x0D\0x0B";
	}

	inline static ch8* GetWhitespaces(ch8* k = 0)
	{
		return "\x20\x09\x0A\x0D\0x0B";
	}

	inline static void ParseValue(ch8* src,int& value)
	{
		//Unformat(src,"%i",&value);
		value = atoi(src);
	}

	inline static void ParseValue(ch16* src,int& value)
	{
		Unformat(src,L"%i",&value);
	}

	inline static void ConvertValue(ch16* dest,int destsize,int value)
	{
		Format(dest,destsize,L"%i",value);
	}

	inline static void ConvertValue(ch8* dest,int destsize,int value)
	{
		Format(dest,destsize,"%i",value);
	}

	inline static void ConvertValue(ch16* dest,int destsize,float value)
	{
		Format(dest,destsize,L"%0.2f",value);
	}

	inline static void ConvertValue(ch8* dest,int destsize,float value)
	{
		Format(dest,destsize,"%0.2f",value);
	}

	inline static void ConvertValue(ch16* dest,int destsize,dword value)
	{
		Format(dest,destsize,L"%u",value);
	}

	inline static void ConvertValue(ch8* dest,int destsize,dword value)
	{
		Format(dest,destsize,"%u",value);
	}

	inline static void Format(ch16* dest,int destsize,const ch16* format, va_list ap)
	{
		_vsnwprintf(dest,destsize,format,ap);
	}

	inline static void Format(ch8* dest,int destsize,const ch8* format, va_list ap)
	{
		vsprintf_s(dest,destsize,format,ap);
	}

	inline static void Unformat(const ch16* src, const ch16* format, va_list ap)
	{
		throw 0;
		swscanf_s(src,format,ap);
	}

	inline static void Unformat(const ch8* src, const ch8* format, va_list ap)
	{
		throw 0;
		sscanf_s(src,format,ap);
	}
	
	inline static void Unformat(const ch16* src, const ch16* format, ...)
	{
		throw 0;
		va_list ap;
		va_start(ap,format);
		Unformat(src,format,ap);
		va_end(ap);
	}

	inline static void Unformat(const ch8* src, const ch8* format, ...)
	{
		throw 0;
		va_list ap;
		va_start(ap,format);
		Unformat(src,format,ap);
		va_end(ap);
	}

	inline static void Format(ch16* dest,int destsize,const ch16* format,...)
	{
		va_list ap;
		va_start(ap,format);
		Format(dest,destsize,format,ap);
		va_end(ap);
	}

	inline static void Format(ch8* dest,int destsize,const ch8* format,...)
	{
		va_list ap;
		va_start(ap,format);
		Format(dest,destsize,format,ap);
		va_end(ap);
	}

	inline static void ConvertCopy(ch16* dst,const ch8* src,int count)
	{
		while(count--)
		{
			*(dst++) = (ch16)(*(src++));
		}
	}

	inline static void ConvertCopy(ch8* dst,const ch16* src,int count)
	{
		while(count--)
		{
			*(dst++) = (ch8)( (*(src++)) & 0xFF );
		}
	}

	inline static ch8* Find(ch8* v1,ch8* v2)
	{
		return strstr(v1,v2);
	}

	inline static ch16* Find(ch16* v1,ch16* v2)
	{
		return wcsstr(v1,v2);
	}

	inline static int Length(ch8* value)
	{
		return strlen(value);
	}

	inline static int Length(ch16* value)
	{
		return wcslen(value);
	}

	inline static ch8* Copy(ch8* dst,ch8* src)
	{
		return strcpy(dst,src);
	}

	inline static ch16* Copy(ch16* dst,ch16* src)
	{
		return wcscpy(dst,src);
	}

	inline static ch8* MemoryCopy(ch8* dst, const ch8* src,int count)
	{
		return (ch8*)strncpy(dst,src,count);
	}

	inline static ch16* MemoryCopy(ch16* dst,const ch16* src,int count)
	{
		return (ch16*)wcsncpy(dst,src,count);
	}

	inline static ch8* Set(ch8* dst,ch8 chr,int count)
	{
		return (ch8*)strnset(dst,chr,count);
	}

	inline static ch16* Set(ch16* dst,ch16 chr,int count)
	{
		return wcsnset(dst,chr,count);
	}

	inline static int Compare(const ch8* v1,const ch8* v2)
	{
		return strcmp(v1,v2);
	}

	inline static int Compare(const ch16* v1,const ch16* v2)
	{
		return wcscmp(v1,v2);
	}

	inline static int Compare(const ch8* v1,const ch8* v2,size_t MaxCount)
	{
		return strncmp(v1,v2,MaxCount);
	}

	inline static int Compare(const ch16* v1,const ch16* v2,size_t MaxCount)
	{
		return wcsncmp(v1,v2,MaxCount);
	}

	inline static ch8* Lower(ch8* value)
	{
		return strlwr(value);
	}

	inline static ch8* Upper(ch8* value)
	{
		return strupr(value);
	}

	inline static ch16* Lower(ch16* value)
	{
		return wcslwr(value);
	}


	inline static ch16* Upper(ch16* value)
	{
		return wcsupr(value);
	}

	inline bool IsWhitespace(ch8 chr)
	{
		switch(chr)
		{
		case 32:
		case 160:
			return true;
		}
		return false;
	}

	inline bool IsWhitespace(ch16 chr)
	{
		if (chr >= 8192 || chr <= 8205)
		{
			return true;
		}
		switch(chr)
		{
		case 32:
		case 160:
		case 5760:
		case 6158:
		case 8239:
		case 8287:
		case 8288:
		case 12288:
		case 65279:
			return true;
		}

		return false;
	}
};

#endif