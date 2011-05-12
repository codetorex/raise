
#ifndef TSTRINGFIXEDWIDTH_H
#define TSTRINGFIXEDWIDTH_H


// THIS SYSTEM IS OBSOLOTE
// WILL BE REMOVED AFTER TSTRING IS REIMPLEMENTED AS UTF8


#include "tarray.h"
#include "tstringdriverfixedwidth.h"

static const char en_chars[62] = // 0,1,2,3 ...a...z....A...Z
{
	48 ,49 ,50 ,51 ,52 ,53 ,54 ,55 ,56 ,57,
	65 ,66 ,67 ,68 ,69 ,70 ,71 ,72 ,73 ,74 ,75 ,76 ,77 ,78 ,79 ,80 ,81 ,82 ,83 ,84 ,85 ,86 ,87 ,88 ,89 ,90,
	97 ,98 ,99 ,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122	
};

// TODO: Implement UTF-8 class its easy, and use it everywhere?
// SOURCES: 
// * http://en.wikipedia.org/wiki/Supplementary_Multilingual_Plane#Supplementary_Multilingual_Plane
// * http://en.wikipedia.org/wiki/UTF-8
// * http://en.wikipedia.org/wiki/Unicode
// * http://www.ietf.org/rfc/rfc2279.txt [RTF-2279] {UTF-8}



/*
	Rules for string class:

	1. Chars will hold the data.
	2. Length is current string length.
	3. Capacity will hold the maximum string capacity without the leading zero.
	   So data allocations should always capacity+1 for leading zero.
	4. For every memory allocations, should use AllocateMemory function.
*/
template <class T>
class RDLL TStringFixedWidth
{
public:
	T*		Chars;
	int		Length;
	int		Capacity;

	TStringFixedWidth()
	{
		Chars = 0;
		Length = 0;
		Capacity = 0;
	}
	
	inline TStringFixedWidth(const T* value)
	{
		Chars = (T*)value;
		Length = StringDriverFixedWidth::Length(Chars);
		Capacity = 0; 
		// Capacity is 0, this means if size changing operations occur, 
		// value will be copied to new buffer so no problems left.
		// this makes me use the "value" char* strings without worried to break anything.
	}

	TStringFixedWidth(T* value)
	{
		Chars = 0;
		Length = 0;
		Capacity = 0;
		Copy(value);
	}

	/**
	* Constructor for already created buffer.
	*/
	TStringFixedWidth(T* buffer,int bufferSize)
	{
		Chars = buffer;
		Capacity = bufferSize;
		Length = 0;
	}

	TStringFixedWidth(int _capacity)
	{
		Capacity = 0;
		Chars = 0;
		Length = 0;
		Allocate(_capacity);
		Clear();
	}

	TStringFixedWidth(int _capacity, T chr)
	{
		Capacity = 0;
		Chars = 0;
		Length = _capacity;
		Allocate(_capacity);
		StringDriverFixedWidth::Set(Chars,chr,_capacity);
	}

	TStringFixedWidth( const TStringFixedWidth<T>& othr) // copy constructor
	{
		Chars = 0;
		Capacity = 0;
		Length = 0;
		*this = othr; // operator = copies the data for us.
	}

	~TStringFixedWidth()
	{
		Free();
	}

	bool HaveChar(T chr) const
	{
		for (int i=0;i<Length;i++)
		{
			T curchr = Chars[i];
			if (curchr == chr)
			{
				return true;
			}
		}
		return false;
	}

	bool HaveCharArray(const TStringFixedWidth<T>& str) const
	{
		for (int i=0;i<Length;i++)
		{
			T curchr = Chars[i];
			for (int k=0;k<str.Length;k++)
			{
				T othchr = str.Chars[k];
				if (curchr == othchr)
				{
					return true;
				}
			}
		}
		return false;
	}

	/**
	* Truncates string without changing capacity.
	*/
	void Truncate(int newLength)
	{
		Length = newLength;
		Chars[Length] = 0;
	}

	void ToLower()
	{
		if (Chars) { StringDriverFixedWidth::Lower(Chars); }
	}

	void ToUpper()
	{
		if (Chars) { StringDriverFixedWidth::Upper(Chars); }
	}
	
	/**
	* Finds a string in string.
	* @param value string to search.
	* @return Index of found string. -1 when not found.
	*/
	inline int IndexOf(T* value)
	{
		return IndexOf(value,0);
	}

	/**
	* Finds a string in string from starting a startindex.
	* @param value string to search.
	* @param startIndex start point.
	* @return Index of found string. -1 when not found.
	*/
	inline int IndexOf(T* value, int startIndex)
	{
		T* kp = StringDriverFixedWidth::Find(Chars+startIndex,value);
		if (kp > 0)
		{
			return kp - Chars;
		}
		else
		{
			return -1;
		}
	}

	/**
	* Finds a string in string from between start index and given count of characters.
	* @param value string to search.
	* @param startIndex start point.
	* @param count search length.
	* @return Index of found string. -1 when not found.
	*/
	int IndexOf(T* value, int startIndex,int count)
	{
		T temp = Chars[startIndex+count];
		Chars[startIndex+count] = 0;
		int k = IndexOf(value,startIndex);
		Chars[startIndex+count] = temp;
		return k;
	}

	inline bool StartsWith(const TStringFixedWidth<T>& value, int startIndex = 0) const
	{
		return ( StringDriverFixedWidth::Compare(Chars+startIndex,value.Chars,value.Length) == 0 );
	}

	inline bool EndsWith(const TStringFixedWidth<T>& value) const
	{
		if (Length < value.Length)
		{
			return false;
		}
		return (StringDriverFixedWidth::Compare((Chars+Length)-value.Length,value.Chars,value.Length) == 0);
	}

	TStringFixedWidth<T> Substring (int startIndex, int lengt) const
	{
		if (startIndex + lengt > Length)
		{
			lengt = Length - startIndex;
		}

		if (lengt == 0)
		{
			return Empty;
		}

		TStringFixedWidth<T> nstring(lengt);
		nstring.Allocate(lengt);
		StringDriverFixedWidth::MemoryCopy(nstring.Chars,Chars+startIndex,lengt);
		nstring.Chars[lengt] = 0;
		nstring.Length = lengt;
		return nstring;
	}

	inline T GetFirst() const
	{
		return Chars[0];
	}

	inline T GetLast() const
	{
		if (Length == 0) return 0;
		return Chars[Length-1];
	}

	TArray<TStringFixedWidth<T>*> Split(T* seprator,bool removeEmpty = false)
	{
		TArray<TStringFixedWidth<T>*> result; //= new TArray<TString<T>*>();

		int sepLength = StringDriverFixedWidth::Length(seprator);
		if (sepLength == 0)
		{
			return result;
		}

		int lastSeprationStart = 0;
		for (int i=0;i<Length;i++)
		{
			T curChar = Chars[i];

			int k=sepLength;
			while(k--)
			{
				if (curChar == seprator[k])
				{
					int seprationLength = i - lastSeprationStart;
					if (removeEmpty && seprationLength == 0 )
					{
						break;
					}
					TStringFixedWidth<T>* str = new TStringFixedWidth<T>();
					*str = Substring(lastSeprationStart,seprationLength);
					result.Add(str);
					lastSeprationStart = i + 1;
				}
			}
		}

		if (lastSeprationStart != Length)
		{
			TStringFixedWidth<T>* str = new TStringFixedWidth<T>();
			*str = Substring(lastSeprationStart,Length-lastSeprationStart);
			result.Add(str);
		}

		return result;
	}

	TArray<TStringFixedWidth<T>>* Split(TArray<TStringFixedWidth<T>>* seprator, bool removeEmpty = false)
	{
		throw "Not Implemented";
	}

	inline TStringFixedWidth<T> Substring ( int startIndex ) const
	{
		return Substring(startIndex,Length - startIndex);
	}

	inline TStringFixedWidth<T>& operator = (T* value)
	{
		Copy(value);
		return *this;
	}

	TStringFixedWidth<T>& operator = (const TStringFixedWidth<T>& value)
	{
		if (value.Length == 0)
		{
			Chars = Empty.Chars;
			Length = 0;
			Capacity = 0;
			return *this;
		}
		Allocate(value.Length);
		StringDriverFixedWidth::MemoryCopy(Chars,value.Chars,value.Length+1);
		Length = value.Length;
		return *this;
	}

	template <class K>
	TStringFixedWidth<T>& operator = (const TStringFixedWidth<K>& value)
	{
		if (value.Length == 0)
		{
			Chars = Empty.Chars;
			Length = 0;
			Capacity = 0;
			return *this;
		}
		Allocate(value.Length);
		StringDriverFixedWidth::ConvertCopy(Chars,value.Chars,value.Length+1);
		Length = value.Length;
		return *this;
	}

	TStringFixedWidth<T>& operator + (T* value)
	{
		int alen = StringDriverFixedWidth::Length(value);
		TStringFixedWidth<T>* nstring = new TStringFixedWidth<T>(alen+Length);
		nstring->Append(Chars,Length);
		nstring->Append(value,alen);
		return *nstring;
	}

	TStringFixedWidth<T>& operator + (TStringFixedWidth<T>& value)
	{
		TStringFixedWidth<T>* nstring = new TStringFixedWidth<T>(value.Length+Length);
		nstring->Append(Chars,Length);
		nstring->Append(value.Chars,value.Length);
		return *nstring;
	}
	
	template <class K>
	TStringFixedWidth<T>& operator + (TStringFixedWidth<K>& value)
	{
		TStringFixedWidth<T>* nstring = new TStringFixedWidth<T>(value.Length+Length);
		nstring->Append(Chars,Length);
		StringDriverFixedWidth::ConvertCopy(Chars+Length,value.Chars,value.Length+1);
		return *nstring;
	}

	inline TStringFixedWidth<T>& operator += (T value)
	{
		Append(&value,1);
		return *this;
	}
	
	inline TStringFixedWidth<T>& operator += (T* value)
	{
		int alen = StringDriverFixedWidth::Length(value);
		Append(value,alen);
		return *this;
	}

	inline TStringFixedWidth<T>& operator += (const TStringFixedWidth<T>& value)
	{
		Append(value.Chars,value.Length);
		return *this;
	}

	template <class K>
	TStringFixedWidth<T>& operator += (const TStringFixedWidth<K>& value)
	{
		if (value.Length == 0) return *this;
		EnsureCapacity(Length+value.Length);
		StringDriverFixedWidth::ConvertCopy(Chars+Length,value.Chars,value.Length+1);
		Length += value.Length;
		return *this;
	}

	TStringFixedWidth<T>& operator += (int value)
	{
		T temp[32];
		StringDriverFixedWidth::ConvertValue(temp,32,value);
		*this += temp;
		return *this;
	}

	TStringFixedWidth<T>& operator += (unsigned long value)
	{
		T temp[32];
		StringDriverFixedWidth::ConvertValue(temp,32,value);
		*this += temp;
		return *this;
	}

	TStringFixedWidth<T>& operator += (float value)
	{
		T temp[32];
		StringDriverFixedWidth::ConvertValue(temp,32,value);
		*this += temp;
		return *this;
	}

	bool operator == (const TStringFixedWidth<T>& value) const
	{
		return (StringDriverFixedWidth::Compare(Chars,value.Chars) == 0);
	}

	bool operator == (const T* value) const
	{
		return (StringDriverFixedWidth::Compare(Chars,value) == 0);
	}

	inline void Clear()
	{
		StringDriverFixedWidth::Set(Chars,0,Capacity);
		Length = 0;
	}


	inline void Trim(T* trimChars = 0)
	{
		if (Length == 0)
		{
			return;
		}
		TrimStart(trimChars);
		TrimEnd(trimChars);
	}

	void TrimStart(T* trimChars = 0)
	{
		if (Length == 0)
		{
			return;
		}

		T k;
		int clen = Length;

		if (trimChars == 0)
		{
			trimChars = StringDriverFixedWidth::GetWhitespaces(trimChars);
		}

		bool contn;
		int trimLen = StringDriverFixedWidth::Length(trimChars);

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
				StringDriverFixedWidth::MemoryCopy(NewChars,Chars+clen,Length+1);
				Use(NewChars,Length);
			}
		}
	}

	void TrimEnd(T* trimChars = 0)
	{
		if (Length == 0)
		{
			return;
		}

		T k;
		int clen = -1;

		if (trimChars == 0)
		{
			trimChars = StringDriverFixedWidth::GetWhitespaces(trimChars);
		}

		bool contn;
		int trimLen = StringDriverFixedWidth::Length(trimChars);

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
				StringDriverFixedWidth::MemoryCopy(NewChars,Chars,Length);
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
			StringDriverFixedWidth::Set(NewChars,padChar,width);
			StringDriverFixedWidth::MemoryCopy(NewChars+padlength,Chars,Length);
			Use(NewChars,width,width);
			return;
		}

		StringDriverFixedWidth::Set(Chars,padChar,width);
		StringDriverFixedWidth::MemoryCopy(Chars+padlength,Chars,Length);
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
			StringDriverFixedWidth::Set(NewChars,padChar,width);
			StringDriverFixedWidth::MemoryCopy(NewChars,Chars,Length);
			Use(NewChars,width,width);
			return;
		}

		StringDriverFixedWidth::Set(Chars,padChar,width);
		StringDriverFixedWidth::MemoryCopy(Chars+padlength,Chars,Length);
	}

	inline int ParseInt() const
	{
		int r;
		StringDriverFixedWidth::ParseValue(Chars,r);
		return r;
	}

	/**
	* You can parse Ints that have alphanumeric characters inside it
	* This function will stop when encounters with an alphanumeric character
	* and returns numericLength via parameter, if not NULL
	* Good for parsing consecutive numbers...
	*/
	int ParseInt(int start,int* numLength = 0, bool skipBegin = true) const
	{
		int numl = 0;

		if (skipBegin)
		{
			for (int i=start;i<Length;i++)
			{
				int r = Chars[i] - 48;
				if (r >= 0 && r <= 9)
				{
					break;
				}
				else
				{
					start++;
				}
			}
		}

		T tmp[16];
		for (int i=start;i<Length;i++)
		{
			int r = Chars[i] - 48;
			if (r >= 0 && r <= 9)
			{
				tmp[numl++] = Chars[i];
			}
			else
			{
				break;
			}
		}
		tmp[numl] = 0;

		int result = 0;
		StringDriverFixedWidth::ParseValue(tmp,result);
		if (numLength != 0)
		{
			*numLength = numl;
		}
		return result;
	}

	static TStringFixedWidth<T> FormatNew(T* format,...)
	{
		T Temp[4096];

		va_list ap;
		va_start(ap,format);
		StringDriverFixedWidth::Format(Temp,4096,format,ap);
		va_end(ap);

		TStringFixedWidth<T> nstring;
		nstring = Temp;
		
		return nstring;
	}

	void Format(T* format, ...)
	{
		va_list ap;
		va_start(ap,format);
		StringDriverFixedWidth::Format(Chars,Capacity,format,ap);
		va_end(ap);
		Length = StringDriverFixedWidth::Length(Chars);
	}


	operator T* (void)
	{
		return Chars;
	}

	static TStringFixedWidth<T> Empty;

	static inline dword GetHash(const TStringFixedWidth<ch8>& value)
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

	static inline void Random(char* chrs,int leng)
	{
		int i = leng;
		while(i--)
		{
			chrs[i] = en_chars[rand() % 62];
		}
	}

	static inline void Random(TStringFixedWidth<ch8>& rnd) // randomize str
	{
		Random(rnd.Chars,rnd.Length);
	}

	static inline TStringFixedWidth<ch8> Random(int rndLength)
	{
		TStringFixedWidth<ch8> rnd(rndLength);
		rnd.Length = rndLength;
		Random(rnd);
		return rnd;
	}

	void Allocate(int newcapacity)
	{
		if (newcapacity <= Capacity) // yeni istenen su ankinden kucukse
		{
			StringDriverFixedWidth::Set(Chars,0,Capacity);
			return;
			/*if (Capacity < newcapacity + 128) // su anki, istenenin 128 fazlasindan kucukse onu kullan
			{
			//memset(Chars,0,Capacity * sizeof(T)); //TODO: you can change this duude
			StringDriver::Set(Chars,0,Capacity);
			//Length = 0; // yeah
			return; // Dont allocate new space if current one is not very big
			}*/
		}

		if (Chars)
		{
			Free();
		}

		if (!newcapacity)
		{
			Capacity = 0;
			Chars = Empty.Chars;
			return;
		}

		Capacity = newcapacity;
		Chars = AllocateMemory(newcapacity);
		Chars[Capacity] = 0;
	}

protected:
	// Memory Operations
	inline T* AllocateMemory(int cap)
	{
		return new T [cap+1];
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

	void Use(T* newbuffer,int newcapacity,int newlength = -1)
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
		Length = StringDriverFixedWidth::Length(value); //wcslen(value);
		Allocate(Length);
		if (Length == 0) return;
		StringDriverFixedWidth::Copy(Chars,value);
		Chars[Length] = 0;
	}

	inline void EnsureCapacity(int cap)
	{
		if (Capacity >= cap)
		{
			return;
		}

		T* NewChars = AllocateMemory(cap);
		StringDriverFixedWidth::MemoryCopy(NewChars,Chars,Length);
		Use(NewChars,cap,Length);
	}

	inline void Append(const T* value,int bufferlength)
	{
		if (bufferlength == 0) return;
		EnsureCapacity(Length+bufferlength);
		StringDriverFixedWidth::MemoryCopy(Chars+Length,value,bufferlength);
		Length += bufferlength;
		Chars[Length] = 0;
	}

};

typedef TStringFixedWidth<ch8>		TString8;	// long bit wise string type
typedef TStringFixedWidth<ch16>		TString16;	// long bit wise string type
typedef TString16			string;		// generic string type

typedef TString8			str8;		// short bit wise string type
typedef TString16			str16;		// short bit wise string type

typedef TString8			str1;		// short byte wise string type
typedef TString16			str2;		// short byte wise string type


extern TString16 CrLf16;
extern TString8 CrLf8;

#endif