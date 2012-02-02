#ifndef TSTRING_H
#define TSTRING_H

#include "tcharacter.h"
#include "tbytearray.h"
#include "tstringdriver.h"
#include "tenumerator.h"
#include "tmemorydriver.h"
#include "mmathdriver.h"


class TStringFormatElementBase;
typedef const TStringFormatElementBase&			sfp; // parameter version

template <class T>
class TArray;

/**
 * UTF-8 based string class.
 * Uses copy on write technique to avoid unnecessary copies.
 */
class TString
{
public:
	/// The character data
	byte* Data;

	/// Byte capacity of Data
	ui32 Capacity;

	/// Character length
	ui32 Length;

	/// Byte length of character data
	ui32 ByteLength;

	TSharedByteArray* Ref; // internal reference for avoid unnecessary copying

private:

	inline void CountCharsAndBytesConst()
	{
		StringDriver::Length(Data,Length,ByteLength);
	}

	inline void StringAllocate(int charCap)
	{
		Data = new byte [charCap + 1]; // for 0 ending
		Capacity = charCap;
	}

	inline void StringAllocateEmpty(int charCap)
	{
		StringAllocate(charCap);
		Data[0] = 0;
	}

	inline void StringAllocateCopy(int newCap,byte* src, int srcLength)
	{
		StringAllocate(newCap);
		if (srcLength > newCap)
		{
			srcLength = newCap;
		}
		if ( srcLength > 0 ) MemoryDriver::Copy(Data,src,srcLength);
		Data[srcLength] = 0;
	}

	inline int InternalIndexOf(const byte* dst, int dstLength , int offset,int limit) const
	{
		return MemoryDriver::SearchVariable(Data + offset,limit,dst,dstLength);
	}

	/**
	 * References this string to another string's data.
	 
	inline void Attach(TSharedByteArray* bRef)
	{
		Ref = bRef;
		Ref->RefCount++;
		Data = Ref->Data;
		Capacity = Ref->Capacity;
		// NOTE: byte length and length should be get from function that refereces from sharedbytearray
	}*/

	inline void CreateRef()
	{
		Ref = new TSharedByteArray(Data,Capacity);
	}

	inline int GetTrimStartPoint(const TArray<ch32>& trimChars, int& bytesToSkip);
	inline int GetTrimEndPoint(const TArray<ch32>& trimChars, int& bytesToSkip);

public:
	/**
	 * Detaches string data from referenced point.
	 * Should be called whenever needed to write to Data.
	 * @param newCap new capacity of detached string.
	 */
	inline void DetachToEdit(ui32 newCap)
	{
		if (Ref)
		{
			if (Ref->RefCount == 1)
			{
				if (newCap > Capacity)
				{
					StringAllocateCopy(newCap,Ref->Data,ByteLength);
					delete Ref;
					CreateRef();
				}
				return;
			}
			else
			{
				Ref->RefCount--;
				if (Ref->RefCount == 0)
				{
					delete Ref;
					Ref = 0;
				}
				StringAllocateCopy(newCap,Ref->Data,ByteLength);
				CreateRef();
			}
		}
		else
		{
			StringAllocateCopy(newCap,Data,ByteLength);
			CreateRef();
		}
	}

	inline void DetachToEdit()
	{
		if (Capacity != 0)
		{
			DetachToEdit(Capacity);
		}
		else
		{
			DetachToEdit(ByteLength);
		}	
	}

private:

	inline void DetachToDestroy()
	{
		if (Ref)
		{
			Ref->RefCount--;
			if (Ref->RefCount == 0)
			{
				delete Ref;
				Ref = 0;
			}
		}
	}

	inline void Append(const byte* src,int length, int charlength)
	{
		ui32 newCap = length + ByteLength;
		if ( newCap >= Capacity-1 )
		{
			newCap += Capacity / 2; // so appending means we gona use a little bit more space
		}
		DetachToEdit(newCap);

		while(length--)
		{
			Data[ByteLength++] = *src++;
		}
		Length += charlength;
		Data[ByteLength] = 0;
	}

public:
	
	// TODO: change name of this function
	inline void CountCharsAndBytes()
	{
		StringDriver::Length(Data,Capacity,Length,ByteLength);
	}

	/// Empty string holder
	static TString Empty;

	/**
	 * @brief String literal constructor.
	 * @param value The value.
	 */
	TString(const char* value)
	{
		Data = (byte*)value;
		CountCharsAndBytesConst();
		Capacity = 0;
		Ref = 0;
	}

	/**
	 * @brief Capacity based constructor for faster operations.
	 * @param _Capacity The capacity + 1 for trailing zero.
	 */
	TString(ui32 ByteCapacity)
	{
		StringAllocateEmpty(ByteCapacity);
		Length = 0;
		ByteLength = 0;
		CreateRef();	
	}


	~TString()
	{
		DetachToDestroy();
	}

	void Allocate(ui32 ByteCapacity)
	{
		DetachToDestroy();
		StringAllocateEmpty(ByteCapacity);
		Length = 0;
		ByteLength = 0;
		CreateRef();
	}

	inline void IncreaseCapacity(ui32 newCapacity)
	{
		DetachToEdit(newCapacity);
	}

	inline void IncreaseCapacity()
	{
		IncreaseCapacity(Capacity * 2);
	}

	inline bool IsASCII() const
	{
		return ByteLength == Length;
	}

	void CopyTo(byte* dst, ui32 bytecount) const
	{
		MemoryDriver::Copy(dst,Data,MathDriver::Min(bytecount,Capacity));
	}

	/**
	 * Checks if this string have specified character.
	 */
	bool Have(ch32 character) const;

	/**
	 * Checks if this string have any character of given string.
	 */
	bool Have(const TString& any) const;

	/**
	 * Truncates this string, without copying new.
	 */
	void Truncate(ui32 newLength)
	{
		if (newLength >= Length) return;
		DetachToEdit(); // detach because we will change data.

		byte* endptr = StringDriver::Count(Data,newLength);
		*endptr = 0;
		Length = newLength;
		ByteLength = endptr - Data;
	}

	/**
	 * Truncates this string, use this function if you know byte length and length.
	 * Useful for if you derived this string from something else and appended something and you want it back.
	 */
	void Truncate(ui32 newLength, ui32 newByteLength)
	{
		DetachToEdit();
		Length = newLength;
		ByteLength = newByteLength;
		Data[ByteLength] = 0;
	}

	/**
	 * Simpler version of Truncate(dword newLength, dword newByteLength).
	 */
	inline void TruncateDerived(const TString& other)
	{
		Truncate(other.Length,other.ByteLength);
	}

	/**
	 * Just adds the character without checking capacity and doesn't cares for references.
	 * Call detach before using this, or just use it while creating strings first time.
	 */
	inline void AppendASCIIFast(char chr)
	{
		Data[ByteLength++] = (byte)chr;
		Length++;
	}

	/**
	 * Appends an ASCII character, so it checks capacity and reference but doesn't struggles with encoding character.
	 */
	inline void AppendASCII( char chr )
	{
		Append((byte*)&chr,1,1);
	}

	inline void AppendUnicodeFast( ch32 chr )
	{
		byte tmp[8];
		byte* src = tmp;
		int ln = StringDriver::Encode(tmp,chr);
		while(ln--)
		{
			Data[ByteLength++] = *src++;
		}
		Length++;
	}

	inline void AppendUnicode( ch32 chr)
	{
		byte tmp[8];
		int ln = StringDriver::Encode(tmp,chr);
		Append(tmp,ln,1); // already detaches
	}

	inline TString& operator += ( ch32 chr )
	{
		AppendUnicode(chr);
		return *this;
	}

	inline TString& operator += ( const TChar& chr )
	{
		AppendUnicode(chr.Character);
		return *this;
	}

	TString& operator += (const ch16* value)
	{
		while(*value > 0)
		{
			AppendUnicode((ch32)*value);
			value++;
		}
		return *this;
	}

	TString ToLower() const;
	TString ToUpper() const;

	void ToLowerInplace();
	void ToUpperInplace();

	/**
	 * @brief Finds a string in string.
	 * @param value string to search.
	 * @return Index of found string. -1 when not found.
	 */
	inline int IndexOf(const TString& value) const
	{
		return InternalIndexOf(value.Data,value.ByteLength,0,ByteLength);
	}

	/**
	 * @brief Finds a string in string from starting a startIndex.
	 * @param value string to search.
	 * @param startIndex start point.
	 * @return Index of found string. -1 when not found.
	 */
	inline int IndexOf(const TString& value, int startIndex) const
	{
		if (IsASCII())
		{
			int result = InternalIndexOf(value.Data,value.ByteLength,startIndex,ByteLength-startIndex);
			if (result != -1)
			{
				result += startIndex;
				return result;
			}
			return -1;
		}
		else
		{
			// TODO: find start index byte start if non ASCII
			throw 0;
		}
	}

	/**
	 * @brief Finds a string in string from between start index and given count of characters.
	 * @param [in] value string to search.
	 * @param startIndex start point.
	 * @param count search length.
	 * @return Index of found string. -1 when not found.
	 */
	inline int IndexOf(const TString& value, int startIndex, int count) const
	{
		if (IsASCII())
		{
			int result = InternalIndexOf(value.Data, value.ByteLength,startIndex, MathDriver::Min<int>(count, ByteLength-startIndex)); //
			if (result != -1)
			{
				result += startIndex;
				return result;
			}
			return -1;
		}
		else
		{
			// TODO: find start index byte start and end if non ASCII
			throw 0;
		}
		
	}

	/**
	 * @brief Determines whether the beginning of this string instance matches the specified string.
	 * @param value The string to be compared.
	 * @param startIndex Start index to compare to self. So you can test "burak" string with ("rak", 2) will return true.
	 * @return true if it succeeds, false if it fails.
	 */
	inline bool StartsWith(const TString& value, int startIndex = 0) const
	{
		if (value.ByteLength > ByteLength) return false;
		return MemoryDriver::Compare(Data + startIndex,value.Data,value.Length) == 0;
	}
	 
	inline bool EndsWith(const TString& value ) const
	{
		if (value.ByteLength > ByteLength) return false;
		return MemoryDriver::Compare((Data + ByteLength) - value.ByteLength,value.Data,value.Length) == 0;
	}

	/**
	 * Gets a part of string from this object. Index is zero based.
	 */
	TString Substring( ui32 startIndex, ui32 lengt ) const;

	TString Substring( ui32 startIndex ) const;



	inline TString Trim()
	{
		return Trim(StringDriver::Whitespaces);
	}

	inline TString TrimStart()
	{
		return TrimStart(StringDriver::Whitespaces);
	}

	inline TString TrimEnd()
	{
		return TrimEnd(StringDriver::Whitespaces);
	}

	inline TString Trim(const TArray<ch32>& trimChars)
	{
		return TrimStart().TrimEnd();
	}

	TString TrimStart(const TArray<ch32>& trimChars);

	TString TrimEnd(const TArray<ch32>& trimChars);

	void TrimStartInplace(const TArray<ch32>& trimChars);
	void TrimEndInplace(const TArray<ch32>& trimChars);
	void TrimInplace(const TArray<ch32>& trimChars);

	void TrimStartInplace()
	{
		TrimStartInplace(StringDriver::Whitespaces);
	}

	void TrimEndInplace()
	{
		TrimEndInplace(StringDriver::Whitespaces);
	}

	void TrimInplace()
	{
		TrimInplace(StringDriver::Whitespaces);
	}

	/**
	 * @brief Gets the first character.
	 * @return The first character.
	 */
	inline ch32 GetFirst() const
	{
		if (ByteLength > 0)
		{
			int blength;
			ch32 result = StringDriver::DecodeFast(Data,blength);
			return result;
		}
		return 0;
	}


	/**
	 * @brief Gets the last character.
	 * @return The last character.
	 */
	inline ch32 GetLast() const
	{
		if (ByteLength > 0)
		{
			int blength;
			ch32 result = StringDriver::ReverseDecode(Data+ByteLength-1,ByteLength,blength);
			return result;
		}
		return 0;
	}

	TArray<TString*> Split(const TArray<ch32>& seprators, bool removeEmpty = false) const;

	TArray<TString*> Split(const TArray<TString*>& seprators, bool removeEMpty = false) const;

	void Clear()
	{
		DetachToEdit(Capacity);
		ByteLength = 0;
		Length = 0;
		Data[ByteLength] = 0;
	}

	inline TString& operator = (const char* value)
	{
		DetachToDestroy();
		Data = (byte*)value;
		Capacity = 0;
		Ref = 0;
		CountCharsAndBytesConst();
		return *this;
	}

	inline TString& operator = (const TString& value)
	{
		//FastCopyDword((dword*)this,(dword*)&value,sizeof(TString) / sizeof(dword));
		DetachToDestroy();

		Data = value.Data;
		Capacity = value.Capacity;
		Length = value.Length;
		ByteLength = value.ByteLength;
		Ref = value.Ref;
		if (Ref)
		{
			Ref->RefCount++;
		}
		return *this;
	}

	TString()
	{
		Ref = 0;
		*this = Empty;
	}

	TString(const TString& value)
	{
		Ref = 0;
		*this = value;
	}

	TString(const byte* value, int length)
	{
		StringAllocateEmpty(length);
		CreateRef();
		MemoryDriver::Copy(Data,value,length);
		Length = StringDriver::Length(Data,Capacity);
		ByteLength = length;
		Data[ByteLength] = 0;
	}

	TString(const byte* value, int length, int charlength)
	{
		StringAllocateEmpty(length);
		CreateRef();
		MemoryDriver::Copy(Data,value,length);
		Length = charlength;
		ByteLength = length;
		Data[ByteLength] = 0;
	}

	inline TString& operator += (const TString& value)
	{
		// this function automaticly detaches, increases capacity when necessary and appends data
		Append(value.Data,value.ByteLength,value.Length);
		return *this;
	}
	
	/*inline TString operator + (const TString& value) const
	{
		TString result(ByteLength + value.ByteLength);
		result += *this;
		result += value;
		return result;
	}*/

	inline bool operator == (const TString& value) const
	{
		if (value.ByteLength != ByteLength)
		{
			return false;
		}
		return MemoryDriver::Compare(Data,value.Data,ByteLength) == 0;
	}

	/**
	 * Formats a string.
	 * Just use % for placement, and parameters will decide how to format.
	 * Use sfi for integer, sfx for hex and so on...
	 */
	static TString Format(const TString& format, int argc , const TStringFormatElementBase** args );

	inline static TString Format(const TString& format, sfp arg0 )
	{
		const TStringFormatElementBase* elemarray[1];
		elemarray[0] = &arg0;
		return Format(format,1,elemarray);
	}

	inline static TString Format(const TString& format, sfp arg0 , sfp arg1)
	{
		const TStringFormatElementBase* elemarray[2];
		elemarray[0] = &arg0;
		elemarray[1] = &arg1;
		return Format(format,2,elemarray);
	}

	inline static TString Format(const TString& format, sfp arg0 , sfp arg1 , sfp arg2)
	{
		const TStringFormatElementBase* elemarray[3];
		elemarray[0] = &arg0;
		elemarray[1] = &arg1;
		elemarray[2] = &arg2;
		return Format(format,3,elemarray);
	}

	inline static TString Format(const TString& format, sfp arg0 , sfp arg1 , sfp arg2 , sfp arg3 )
	{
		const TStringFormatElementBase* elemarray[4];
		elemarray[0] = &arg0;
		elemarray[1] = &arg1;
		elemarray[2] = &arg2;
		elemarray[3] = &arg3;
		return Format(format,4,elemarray);
	}

	//void FormatInplace(const TString& format, ... );

	/*inline static TString Format(const TString& format,...)
	{
		va_list args;
		va_start (args, format);
		vsprintf (buffer,format, args);
		perror (buffer);
		vsnprintf()
		va_end (args);
	}*/
};

/**
 * Can add char* + tstring, or tstring + char* brilliant isn't it?
 */
inline TString operator + (const TString& value, const TString& value2 )
{
	TString tmp = value;
	tmp += value2;
	return tmp;
}

class TCharacterReverseEnumerator: public TEnumerator<ch32>
{
public:
	inline void Reset()
	{
		Current = MXDWORD;
		StrData = SrcString->Data;
		EndData = StrData + SrcString->ByteLength - 1;
		CharIndex = SrcString->Length;
	}

	inline bool MoveNext()
	{
		if (EndData <= StrData)
		{
			return false;
		}
		int lng;
		Current = StringDriver::ReverseDecode(EndData,EndData - StrData, lng );
		EndData -= lng;
		CharIndex--;
		return true;
	}

	ch32 ReadChar()
	{
		MoveNext();
		return Current;
	}

	TCharacterReverseEnumerator(const TString& src)
	{
		SrcString = &src;
		Reset();
	}

	const TString* SrcString;
	byte* StrData;
	byte* EndData;
	int CharIndex;
};

class TCharacterEnumerator: public TEnumerator<ch32>
{
public:
	inline void Reset()
	{
		Current = MXDWORD;
		StrData = SrcString->Data;
		EndData = StrData + SrcString->ByteLength;
		CharIndex = 0;
	}

	inline bool MoveNext()
	{
		if (StrData >= EndData)
		{
			return false;
		}
		Current = StringDriver::DecodeAdv(StrData);
		CharIndex++;
		return true;
	}

	inline void SkipChars(ui32 charsToSkip)
	{
		if (charsToSkip >= SrcString->Length - CharIndex)
		{
			StrData = EndData;
			return;
		}
		if (SrcString->IsASCII())
		{
			StrData += charsToSkip;
		}
		else
		{
			StrData = StringDriver::Count(StrData,charsToSkip);
		}
		CharIndex += charsToSkip;
	}

	ch32 ReadChar()
	{
		MoveNext();
		return Current;
	}

	TCharacterEnumerator(const TString& src)
	{
		SrcString = &src;
		Reset();
	}

	const TString* SrcString;
	byte* StrData;
	byte* EndData;
	int CharIndex;
};

typedef TString string;

#endif