#ifndef TCONVERT_H
#define TCONVERT_H

#include "tstring.h"
#include "texception.h"

class TConvert
{
public:


	/**
	 * Converts int value to character array and returns characters written.
	 * To be safe keep dst at least 12 bytes length.
	 * Warning: Doesn't puts 0 to ending.
	 */
	static int ToCharArrayInt(int value, char* dst, int dstCap);

	inline static int ToCharArrayUInt(ui32 value, char* dst, int dstCap)
	{
		return sprintf(dst,"%u",value);
	}

	inline static int ToCharArrayUIntHex(ui32 value, char* dst, int dstCap)
	{
		return sprintf(dst,"%x",value);
	}

	inline static int ToCharArrayUIntHexCapital(ui32 value, char* dst, int dstCap)
	{
		return sprintf(dst,"%X",value);
	}

	inline static int ToCharArrayFloat(float value, char* dst, int dstCap, int precision = 0)
	{
		// I am not going to implement this function at this moment
		// Instead I will cheat >;) 

		if (precision == 0)
		{
			return sprintf(dst,"%f", value);
		}
		else
		{
			return sprintf(dst,"%.*f", precision , value);
		}
	}

	/**
	 * @brief Converts the value of the specified 32-bit signed integer to its equivalent string representation.
	 * @param value 32-bit signed integer
	 * @return String representation of value
	 */
	static inline TString ToString(int value)
	{
		TString result(16);
		int ln = ToCharArrayInt(value,(char*)result.Data,16);
		result.Length = ln;
		result.ByteLength = ln;
		result.Data[ln] = 0;
		return result;
	}

	static inline void AppendToString(int value, TString& output)
	{
		int ln = ToCharArrayInt(value,(char*)output.Data + output.ByteLength,output.Capacity);
		output.Length += ln;
		output.ByteLength += ln;
		output.Data[output.ByteLength] = 0;
	}
	
	static TString ToString(int value, int base)
	{
		throw NotImplementedException();
	}

	static int ToInt32(char* value);

	static int ToInt32(const TString& value);

	static int  ToInt32Ambiguous(const TString& value, int startIndex, int* numLength = 0, bool skipBegin = true);
};

typedef TConvert Convert;


#endif