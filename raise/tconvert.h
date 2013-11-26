#ifndef TCONVERT_H
#define TCONVERT_H


#ifdef LINUX
#define sprintf_s snprintf
#endif

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
		return sprintf_s(dst,dstCap,"%u",value);
	}

	inline static int ToCharArrayUIntHex(ui32 value, char* dst, int dstCap)
	{
		return sprintf_s(dst,dstCap,"%x",value);
	}

	inline static int ToCharArrayUIntHexCapital(ui32 value, char* dst, int dstCap)
	{
		return sprintf_s(dst,dstCap,"%X",value);
	}

	inline static int ToCharArrayFloat(float value, char* dst, int dstCap, int precision = 0)
	{
		// I am not going to implement this function at this moment
		// Instead I will cheat >;) 

		if (precision == 0)
		{
			return sprintf_s(dst,dstCap,"%f", value);
		}
		else
		{
			return sprintf_s(dst,dstCap,"%.*f", precision , value);
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
		throw NotImplementedException(__FILE__,__LINE__);
	}

	static int ToInt32(char* value);

	static int ToInt32(const TString& value);

	static ui32 DecodeHexChar(char val);

	static ui32 ToUInt32Hex(const TString& value);

	static int ToInt32Ambiguous(const TString& value, int startIndex, int* numLength = 0, bool skipBegin = true);

	/**
	 * Splits a string with a character, then parses integers available. Returns match count, and adds integers to results array
	 * Warning: skips non number files between splits like
	 * example: "54 4845/ 58 539 / 48482" as value, and splitchar is '/' it will parse ints as 544845,58539,48482
	 */
	static int ToInt32Split(const TString& value, ch32 splitChar, TArray<int>& results);
};

typedef TConvert Convert;


#endif