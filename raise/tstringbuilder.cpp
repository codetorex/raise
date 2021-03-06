#include "stdafx.h"
#include "tstring.h"


String TStringBuilder::ToString( int startIndex, int length )
{
	if ( ByteLength == Length)
	{
		String result(length+1);
		MemoryDriver::Copy(result.Data,Data+startIndex,length);
		result.ByteLength = length;
		result.Length = length;
		return result;
	}

	// Pseudo code
	// a = Count bytes of charcters from 0 to startIndex
	// b = Count bytes of characters from startIndex to startIndex + length
	// Copy (b) bytes from (a) to new string

	throw NotImplementedException(__FILE__,__LINE__);
}

String TStringBuilder::ToString()
{
	String result(ByteLength);
	MemoryDriver::Copy(result.Data,Data,ByteLength);
	result.ByteLength = ByteLength;
	result.Length = Length;
	result.Data[result.ByteLength] = 0;
	return result;
}