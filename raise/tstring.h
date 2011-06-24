#ifndef TSTRING_H
#define TSTRING_H

#include "tcharacter.h"
#include "tbytearray.h"
#include "tstringdriver.h"
#include "tenumerator.h"
#include "tmemorydriver.h"
#include "tarray.h"


/**
 * UTF-8 based string class.
 * Uses copy on write technique to avoid unecessary copies.
 */
class TString
{
public:
	/// The character data
	byte* Data;

	/// Byte capacity of Data
	dword Capacity;

	/// Character length
	dword Length;

	/// Byte length of character data
	dword ByteLength;

	TSharedByteArray* Ref; // internal reference for avoid unnecessary copying

private:
	inline void CountCharsAndBytes()
	{
		StringDriver::Length(Data,Capacity,Length,ByteLength);
	}

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
		MemoryDriver::Copy(Data,src,srcLength);
		Data[srcLength] = 0;
	}

	inline int IndexOf(const byte* dst, int length ) const
	{
		return MemoryDriver::SearchVariable(Data,ByteLength,dst,length);
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

	/**
	 * Detaches string data from referenced point.
	 * Should be called whenever needed to write to Data.
	 * @param newCap new capacity of detached string.
	 */
	inline void DetachToEdit(dword newCap)
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

	inline void DetachToDestroy()
	{
		if (Ref)
		{
			Ref->RefCount--;
			if (Ref->RefCount == 0)
			{
				delete Ref;
			}
		}
	}

	inline void Append(const byte* src,int length)
	{
		int newCap = length + ByteLength;
		if ( newCap >= Capacity-1 )
		{
			newCap += Capacity / 2; // so appending means we gona use a little bit more space
		}
		DetachToEdit(newCap);

		while(length--)
		{
			Data[ByteLength++] = *src++;
		}
	}

	inline void FastCopyDword(dword* dst,dword* src,int cnt)
	{
		while(cnt--)
		{
			*(dst++) = *(src++);
		}
	}

public:

	static TString Empty;

	TString()
	{
		Data = 0;
		Capacity = 0;
		Length = 0;
		ByteLength = 0;
		Ref = 0;
	}

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
	TString(dword ByteCapacity)
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

	bool Have(ch32 character) const;
	bool Have(const TString& any) const;

	/**
	 * Truncates this string, without copying new.
	 */
	void Truncate(dword newLength)
	{
		if (newLength >= Length) return;
		DetachToEdit(Capacity); // detach because we gona change data.

		byte* endptr = StringDriver::Count(Data,newLength);
		*endptr = 0;
		Length = newLength;
		ByteLength = endptr - Data;
	}

	inline TString& operator += (const TChar& chr)
	{
		byte tmp[8];
		int ln = StringDriver::Encode(tmp,chr.Character);
		Append(tmp,ln); // already detaches
		return *this;
	}

	TString ToLower() const;
	TString ToUpper() const;

	/**
	 * @brief Finds a string in string.
	 * @param value string to search.
	 * @return Index of found string. -1 when not found.
	 */
	inline int IndexOf(const TString& value) const
	{
		return IndexOf(value.Data,value.ByteLength);
	}

	/**
	 * @brief Finds a string in string from starting a startindex.
	 * @param value string to search.
	 * @param startIndex start point.
	 * @return Index of found string. -1 when not found.
	 */
	inline int IndexOf(const TString& value, int startIndex) const
	{
		return IndexOf(value.Data + startIndex,value.ByteLength - startIndex);
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
		return IndexOf(value.Data + startIndex, min(count,ByteLength - startIndex));
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

	TString Substring(int startIndex, int lengt) const;

	inline TString Substring(int startIndex) const
	{
		return Substring(startIndex,Length - startIndex);
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
			ch32 result = StringDriver::Decode(Data,blength);
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
			ch32 result = StringDriver::ReverseDecode(Data+ByteLength,ByteLength,blength);
			return result;
		}
		return 0;
	}


	TArray<TString*>* Split(ch32* seprators, bool removeEmpty = false) const;

	TArray<TString*>* Split(const TArray<TString*>& seprators, bool removeEMpty = false) const;

	inline TString& operator = (const char* value)
	{
		DetachToDestroy();
		Data = (byte*)value;
		Capacity = 0;
		Ref = 0;
		CountCharsAndBytesConst();
	}

	inline TString& operator = (const TString& value)
	{
		//FastCopyDword((dword*)this,(dword*)&value,sizeof(TString) / sizeof(dword));

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

	TString(const TString& value)
	{
		*this = value;
	}

	inline TString& operator += (const TString& value)
	{
		// this function automaticly detaches, increases capacity when necessary and appends data
		Append(value.Data,value.ByteLength);
		return *this;
	}
	
	inline TString operator + (const TString& value) const
	{
		TString result(ByteLength + value.ByteLength);
		result += *this;
		result += value;
		return result;
	}

	inline bool operator == (const TString& value) const
	{
		if (value.ByteLength != ByteLength)
		{
			return false;
		}
		return MemoryDriver::Compare(Data,value.Data,ByteLength) == 0;
	}
};


typedef TString string;

#endif