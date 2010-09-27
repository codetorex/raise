
#ifndef TSTRING_H
#define TSTRING_H

#include "raisetypes.h"
#include <memory>

typedef wchar_t ch16;
typedef char	ch8;

class StringDriver
{
public:
	inline static void Format(ch16* dest,ch16* format,...)
	{
		va_list ap;
		va_start(ap,format);
		_vsnwprintf(dest,1024,format,ap);
		va_end(ap);
	}

	inline static void Format(ch8* dest,ch8* format,...)
	{
		va_list ap;
		va_start(ap,format);
		vsprintf(dest,format,ap);
		va_end(ap);
	}

	inline static void ConvertCopy(ch16* dst,ch8* src,int count)
	{
		while(count--)
		{
			*(dst++) = (ch16)(*(src++));
		}
	}

	inline static void ConvertCopy(ch8* dst,ch16* src,int count)
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

	inline static ch8* MemoryCopy(ch8* dst,ch8* src,int count)
	{
		return (ch8*)memcpy(dst,src,count);
	}

	inline static ch16* MemoryCopy(ch16* dst,ch16* src,int count)
	{
		return (ch16*)memcpy(dst,src,count<<1);
	}

	inline static ch8* Set(ch8* dst,ch8 chr,int count)
	{
		return (ch8*)memset(dst,chr,count);
	}

	inline static ch16* Set(ch16* dst,ch16 chr,int count)
	{
		return (ch16*)memset(dst,chr,count<<1);
	}

	inline static int Compare(ch8* v1,ch8* v2)
	{
		return strcmp(v1,v2);
	}

	inline static int Compare(ch16* v1,ch16* v2)
	{
		return wcscmp(v1,v2);
	}

	inline static ch8* Lower(ch8* value)
	{
		return _strlwr(value);
	}

	inline static ch16* Lower(ch16* value)
	{
		return _wcslwr(value);
	}

	inline static ch8* Upper(ch8* value)
	{
		return _strupr(value);
	}

	inline static ch16* Upper(ch16* value)
	{
		return _wcsupr(value);
	}
};

/*
	Rules for string class:

	1. Chars will hold the data.
	2. Length is current string length.
	3. Capacity will hold the maximum string capacity without the leading zero.
	   So data allocations should always capacity+1 for leading zero.
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

	TString(T* value)
	{
		Chars = 0;
		Length = 0;
		Capacity = 0;
		Copy(value);
	}

	TString(int capacity)
	{
		Length = 0;
		Capacity = 0;
		Chars = 0;
		Allocate(ncapacity);
		Clear();
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
		TString<T>* nstring = new TString<T>;
		nstring->Allocate(lengt+1);
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

	TString<T>& operator = (TString<T>& value)
	{
		Allocate(value.Length+1);
		StringDriver::MemoryCopy(Chars,value.Chars,value.Length+1);
		Length = value.Length;
		return *this;
	}

	template <class K>
	TString<T>& operator = (TString<K>& value)
	{
		Allocate(value.Length+1);
		StringDriver::ConvertCopy(Chars,value.Chars,value.Length+1);
		Length = value.Length;
		return *this;
	}

	TString<T>& operator + (T* value)
	{
		int alen = StringDriver::Length(value);
		TString<T>* nstring = new TString<T>(alen+Length+1);
		nstring->Append(Chars,Length);
		nstring->Append(value,alen);
		return *nstring;
	}

	TString<T>& operator + (TString<T>& value)
	{
		TString<T>* nstring = new TString<T>(value.Length+Length+1);
		nstring->Append(Chars,Length);
		nstring->Append(value.Chars,value.Length);
		return *nstring;
	}
	
	template <class K>
	TString<T>& operator + (TString<K>& value)
	{
		TString<T>* nstring = new TString<T>(value.Length+Length+1);
		nstring->Append(Chars,Length);
		StringDriver::ConvertCopy(Chars+Length,value.Chars,value.Length);
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
		TString<T>* nstring = new TString<T>(value.Length+Length+1);
		nstring->Append(Chars,Length);
		StringDriver::ConvertCopy(Chars+Length,value.Chars,value.Length);
		return *nstring;
	}


	TString<T>& operator += (int value)
	{
		T temp[32];
		StringDriver::Format(temp,"%i",value);
		*this += temp;
		return *this;
	}

	TString<T>& operator += (unsigned long value)
	{
		T temp[32];
		StringDriver::Format(temp,"%u",value);
		*this += temp;
		return *this;
	}

	TString<T>& operator += (float value)
	{
		T temp[32];
		StringDriver::Format(temp,"%0.2f",value);
		*this += temp;
		return *this;
	}

	bool operator == (TString<T>& value)
	{
		return (StringDriver::Compare(Chars,value.Chars) == 0);
	}

	bool operator == (T* value)
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
			trimChars = L"\x20\x09\x0A\x0D\0x0B";
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
				Allocate(1);
				Chars[0] = 0; // capacity will remain same
			}
			else
			{
				T* NewChars = new T [Length+1];
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
			trimChars = L"\x20\x09\x0A\x0D\0x0B";
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
			Allocate(1);
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
				T* NewChars = new T [Length+1];
				StringDriver::MemoryCopy(NewChars,Chars,Length);
				Use(NewChars,Length);
				NewChars[Length] = 0;
			}
		}
	}


	void PadLeft(int width)
	{
		PadLeft(width,' ');
	}

	void PadLeft(int width,T padChar)
	{
		if (width < Length) return;
		
		if (Capacity <= width)
		{
			T* NewChars = new T [width+1];

		}

	}

	void PadRight(int width)
	{
		PadRight(width,' ');
	}

	void PadRight(int width,T padChar)
	{

	}


	operator T* (void)
	{
		return Chars;
	}

	/*
	void Trim(T* trimChars = 0);
	void TrimStart(T* trimChars = 0);
	void TrimEnd(T* trimChars = 0);

	TString<char>& Format(T* fmt,...);
*/

protected:
	// Memory Operations
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

		Capacity = newcapacity-1;
		Chars = new T [newcapacity];
		Chars[Capacity] = 0;
	}

	inline void Free()
	{
		if (Chars)
		{
			delete [] Chars;
		}
		Chars = 0;
		Capacity = 0;
		Length = 0;
	}

	inline void Use(T* newbuffer,int newcapacity,int newlength = -1)
	{
		if (Chars)
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
		Allocate(Length+1);
		StringDriver::Copy(Chars,value);
		Chars[Length] = 0;
	}

	inline void Append(T* value,int bufferlength)
	{
		if (Capacity >= (Length + bufferlength)) // if have enough capacity
		{
			StringDriver::MemoryCopy(Chars+Length,value,bufferlength);
			Length += bufferlength;	//NOTE: while writing for unicode this may need to add half of it?
		}
		else
		{
			T* NewChars = new T [Length+bufferlength+1];
			StringDriver::MemoryCopy(NewChars,Chars,Length);
			StringDriver::MemoryCopy(NewChars+Length,value,bufferlength);
			Use(NewChars,Length+bufferlength);
		}
		Chars[Length] = 0;
	}

};

typedef TString<char>		TString8;
typedef TString<wchar_t>	TString16;
typedef TString16			string;

extern TString16 CrLf16;
extern TString8 CrLf8;

#endif