
#ifndef TSTRING_H
#define TSTRING_H

#include "raisetypes.h"

#include <string.h>

#ifdef WIN32
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

typedef wchar_t ch16;
typedef char	ch8;

static const char en_chars[62] = // 0,1,2,3 ...a...z....A...Z
{
	48 ,49 ,50 ,51 ,52 ,53 ,54 ,55 ,56 ,57,
	65 ,66 ,67 ,68 ,69 ,70 ,71 ,72 ,73 ,74 ,75 ,76 ,77 ,78 ,79 ,80 ,81 ,82 ,83 ,84 ,85 ,86 ,87 ,88 ,89 ,90,
	97 ,98 ,99 ,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122	
};

class StringDriver
{
public:

	inline const ch16* GetWhitespaces(ch16* k = 0)
	{
		return L"\x20\x09\x0A\x0D\0x0B";
	}

	inline const ch8* GetWhitespaces(ch8* k = 0)
	{
		return "\x20\x09\x0A\x0D\0x0B";
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

	inline static void ConvertValue(ch16* dest,int destsize,unsigned long value)
	{
		Format(dest,destsize,L"%u",value);
	}

	inline static void ConvertValue(ch8* dest,int destsize,unsigned long value)
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

};

/*
	Rules for string class:

	1. Chars will hold the data.
	2. Length is current string length.
	3. Capacity will hold the maximum string capacity without the leading zero.
	   So data allocations should always capacity+1 for leading zero.
	4. For every memory allocations, should use AllocateMemory function.
*/


template <class T>
class RDLL TString
{
public:
	T*		Chars;
	int		Length;
	int		Capacity;

	TString()
	{
		Chars = 0;
		Length = 0;
		Capacity = 0;
	}
	
	TString(const T* value)
	{
		Chars = (T*)value;
		Length = StringDriver::Length(Chars);
		Capacity = 0; 
		// Capacity is 0, this means if size changing operations occur, 
		// value will be copied to new buffer so no problems left.
		// this makes me use the "value" char* strings without worried to break anything.
	}

	TString(T* value)
	{
		Chars = 0;
		Length = 0;
		Capacity = 0;
		Copy(value);
	}

	TString(int _capacity)
	{
		Capacity = 0;
		Chars = 0;
		Length = 0;
		Allocate(_capacity);
		Clear();
	}

	TString(int _capacity, T chr)
	{
		Capacity = 0;
		Chars = 0;
		Length = _capacity;
		Allocate(_capacity);
		StringDriver::Set(Chars,chr,_capacity);
	}

	TString( const TString<T>& othr) // copy constructor.
	{
		Chars = 0;
		Capacity = 0;
		Length = 0;
		*this = othr;
	}

	~TString()
	{
		Free();
	}

	void ToLower()
	{
		if (Chars) { StringDriver::Lower(Chars); }
	}

	void ToUpper()
	{
		if (Chars) { StringDriver::Upper(Chars); }
	}
	
	int IndexOf(T* value)
	{
		return IndexOf(value,0);
	}

	int IndexOf(T* value, int startIndex)
	{
		T* kp = StringDriver::Find(Chars+startIndex,value);
		if (kp > 0)
		{
			return kp - Chars;
		}
		else
		{
			return -1;
		}
	}

	int IndexOf(T* value, int startIndex,int count)
	{
		T temp = Chars[startIndex+count];
		Chars[startIndex+count] = 0;
		int k = IndexOf(value,startIndex);
		Chars[startIndex+count] = temp;
		return k;
	}

	TString<T>& Substring (int startIndex, int lengt)
	{
		if (startIndex + lengt > Length)
		{
			lengt = Length - startIndex;
		}
		// TODO: If lengt is 0 do something else

		TString<T>* nstring = new TString<T>(lengt);
		nstring->Allocate(lengt);
		StringDriver::MemoryCopy(nstring->Chars,Chars+startIndex,lengt);
		nstring->Chars[lengt] = 0;
		nstring->Length = lengt;
		return *nstring;
	}

	TString<T>& Substring ( int startIndex )
	{
		return Substring(startIndex,Length - startIndex);
	}

	TString<T>& operator = (T* value)
	{
		Copy(value);
		return *this;
	}

	TString<T>& operator = (const TString<T>& value)
	{
		Allocate(value.Length);
		StringDriver::MemoryCopy(Chars,value.Chars,value.Length+1);
		Length = value.Length;
		return *this;
	}

	template <class K>
	TString<T>& operator = (const TString<K>& value)
	{
		Allocate(value.Length);
		StringDriver::ConvertCopy(Chars,value.Chars,value.Length+1);
		Length = value.Length;
		return *this;
	}

	TString<T>& operator + (T* value)
	{
		int alen = StringDriver::Length(value);
		TString<T>* nstring = new TString<T>(alen+Length);
		nstring->Append(Chars,Length);
		nstring->Append(value,alen);
		return *nstring;
	}

	TString<T>& operator + (TString<T>& value)
	{
		TString<T>* nstring = new TString<T>(value.Length+Length);
		nstring->Append(Chars,Length);
		nstring->Append(value.Chars,value.Length);
		return *nstring;
	}
	
	template <class K>
	TString<T>& operator + (TString<K>& value)
	{
		TString<T>* nstring = new TString<T>(value.Length+Length);
		nstring->Append(Chars,Length);
		StringDriver::ConvertCopy(Chars+Length,value.Chars,value.Length+1);
		return *nstring;
	}
	
	TString<T>& operator += (T* value)
	{
		int alen = StringDriver::Length(value);
		Append(value,alen);
		return *this;
	}

	TString<T>& operator += (TString<T>& value)
	{
		Append(value.Chars,value.Length);
		return *this;
	}

	template <class K>
	TString<T>& operator += (TString<K>& value)
	{
		EnsureCapacity(Length+value.Length);
		StringDriver::ConvertCopy(Chars+Length,value.Chars,value.Length+1);
		Length += value.Length;
		return *this;
	}

	TString<T>& operator += (int value)
	{
		T temp[32];
		StringDriver::ConvertValue(temp,32,value);
		*this += temp;
		return *this;
	}

	TString<T>& operator += (unsigned long value)
	{
		T temp[32];
		StringDriver::ConvertValue(temp,32,value);
		*this += temp;
		return *this;
	}

	TString<T>& operator += (float value)
	{
		T temp[32];
		StringDriver::ConvertValue(temp,32,value);
		*this += temp;
		return *this;
	}

	bool operator == (const TString<T>& value) const
	{
		return (StringDriver::Compare(Chars,value.Chars) == 0);
	}

	bool operator == (const T* value)
	{
		return (StringDriver::Compare(Chars,value) == 0);
	}

	void Clear()
	{
		StringDriver::Set(Chars,0,Capacity);
		Length = 0;
	}


	void Trim(T* trimChars /* = 0 */)
	{
		TrimStart(trimChars);
		TrimEnd(trimChars);
	}

	void TrimStart(T* trimChars = 0)
	{
		T k;
		int clen = Length;

		if (trimChars == 0)
		{
			trimChars = StringDriver::GetWhitespaces(Chars);
		}

		bool contn;
		int trimLen = StringDriver::Length(trimChars);

		for (int i=0;i<Length;i++)
		{
			k = Chars[i];
			contn = false;
			for (int j=0;j<trimLen;j++)
			{
				if (k == trimChars[j])
				{
					contn = true;
					break;
				}
			}

			if (!contn)
			{
				clen = i;
				break;
			}
		}

		if (clen == 0)
		{
			return;
		}
		else
		{
			Length -= clen;
			if (Length == 0)
			{
				Allocate(0);
				Chars[0] = 0; // capacity will remain same
			}
			else
			{
				T* NewChars = AllocateMemory(Length);
				StringDriver::MemoryCopy(NewChars,Chars+clen,Length+1);
				Use(NewChars,Length);
			}
		}
	}

	void TrimEnd(T* trimChars = 0)
	{
		T k;
		int clen = -1;

		if (trimChars == 0)
		{
			trimChars = StringDriver::GetWhitespaces(Chars);
		}

		bool contn;
		int trimLen = StringDriver::Length(trimChars);

		for (int i=Length-1;i>=0;i--)
		{
			k = Chars[i];
			contn = false;
			for (int j=0;j<trimLen;j++)
			{
				if (k == trimChars[j])
				{
					contn = true;
					break;
				}
			}

			if (!contn)
			{
				clen = i + 1;
				break;
			}
		}

		if (clen == -1)
		{
			Allocate(0);
			Chars[0] = 0; // capacity will remain same
		}
		else
		{
			int OldLength = Length;
			Length = clen;

			if (Length == OldLength)
			{
				return; // nothing was changed lol
			}
			else
			{
				T* NewChars = AllocateMemory(Length);
				StringDriver::MemoryCopy(NewChars,Chars,Length);
				Use(NewChars,Length);
				NewChars[Length] = 0;
			}
		}
	}

	inline void PadLeft(int width)
	{
		PadLeft(width,' '); // ' ' is equal in wchar and char
	}

	void PadLeft(int width,T padChar)
	{
		if (width < Length) return;
		int padlength = width - Length;

		if (Capacity <= width)
		{
			T* NewChars = AllocateMemory(width);
			StringDriver::Set(NewChars,padChar,width);
			StringDriver::MemoryCopy(NewChars+padlength,Chars,Length);
			Use(NewChars,width,width);
			return;
		}

		StringDriver::Set(Chars,padChar,width);
		StringDriver::MemoryCopy(Chars+padlength,Chars,Length);
	}

	inline void PadRight(int width)
	{
		PadRight(width,' ');
	}

	void PadRight(int width,T padChar)
	{
		if (width < Length) return; // actually an exception should occur
		int padlength = width - Length;

		if (Capacity <= width)
		{
			T* NewChars = AllocateMemory(width);
			StringDriver::Set(NewChars,padChar,width);
			StringDriver::MemoryCopy(NewChars,Chars,Length);
			Use(NewChars,width,width);
			return;
		}

		StringDriver::Set(Chars,padChar,width);
		StringDriver::MemoryCopy(Chars+padlength,Chars,Length);
	}

	static TString<T>& Format(T* format,...)
	{
		T Temp[4096];

		va_list ap;
		va_start(ap,format);
		StringDriver::Format(Temp,4096,format,ap);
		va_end(ap);

		TString<T>* nstring = new TString<T>;
		*nstring = Temp;
		
		return *nstring;
	}


	operator T* (void)
	{
		return Chars;
	}

	static inline dword GetHash(const TString<ch8>& value)
	{
		register dword hash = 0;
		int i = value.Length;
		while(i--)
		{
			hash += value.Chars[i];
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}

	static inline TString<ch8>& Random(int rndLength)
	{
		TString<ch8> rnd(rndLength);
		Random(rnd);
		return rnd;
	}

	static inline void Random(char* chrs,int leng)
	{
		int i = leng;
		while(i--)
		{
			chrs[i] = en_chars[rand() % 62];
		}
	}

	static inline void Random(TString<ch8>& rnd) // randomize str
	{
		Random(rnd.Chars,rnd.Length);
	}

protected:
	// Memory Operations
	inline T* AllocateMemory(int cap)
	{
		return new T [cap+1];
	}

	inline void Allocate(int newcapacity)
	{
		if (newcapacity < Capacity) // yeni istenen su ankinden kucukse
		{
			if (Capacity < newcapacity + 128) // su anki, istenenin 128 fazlasindan kucukse onu kullan
			{
				//memset(Chars,0,Capacity * sizeof(T)); //TODO: you can change this duude
				StringDriver::Set(Chars,0,Capacity);
				//Length = 0; // yeah
				return; // Dont allocate new space if current one is not very big
			}
		}

		if (Chars)
		{
			Free();
		}

		if (!newcapacity)
		{
			return;
		}

		Capacity = newcapacity;
		Chars = AllocateMemory(newcapacity);
		Chars[Capacity] = 0;
	}

	inline void Free()
	{
		if (Chars && Capacity > 0)
		{
			delete [] Chars;
		}
		Chars = 0;
		Capacity = 0;
		Length = 0;
	}

	inline void Use(T* newbuffer,int newcapacity,int newlength = -1)
	{
		if (Chars) // so we will not accidentally try to free CONST * s
		{
			Free();
		}
		Chars = newbuffer;
		Capacity = newcapacity;
		if (newlength == -1)
		{
			Length = Capacity;
		}
		else
		{
			Length = newlength; // hmm
		}
		Chars[Length] = 0;
	}

	inline void Copy (T* value)
	{
		Length = StringDriver::Length(value); //wcslen(value);
		Allocate(Length);
		StringDriver::Copy(Chars,value);
		Chars[Length] = 0;
	}

	inline void EnsureCapacity(int cap)
	{
		if (Capacity >= cap)
		{
			return;
		}

		T* NewChars = AllocateMemory(cap);
		StringDriver::MemoryCopy(NewChars,Chars,Length);
		Use(NewChars,cap,Length);
	}

	inline void Append(T* value,int bufferlength)
	{
		EnsureCapacity(Length+bufferlength);
		StringDriver::MemoryCopy(Chars+Length,value,bufferlength);
		Length += bufferlength;
		Chars[Length] = 0;
	}

};

typedef TString<ch8>		TString8;	// long bit wise string type
typedef TString<ch16>		TString16;	// long bit wise string type
typedef TString16			string;		// generic string type

typedef TString8			str8;		// short bit wise string type
typedef TString16			str16;		// short bit wise string type

typedef TString8			str1;		// short byte wise string type
typedef TString16			str2;		// short byte wise string type


extern TString16 CrLf16;
extern TString8 CrLf8;

#endif