#include "stdafx.h"
#include "tstring.h"
#include "tutf16encoding.h"
#include "texception.h"

TString TString::Empty("");

bool TString::Have( ch32 character ) const
{
	TCharacterEnumerator schars(*this);
	while(schars.MoveNext())
	{
		if (schars.Current == character)
		{
			return true;
		}
	}
	return false;

	/*byte* start = Data;
	ch32 curChr;
	do 
	{
		curChr = StringDriver::DecodeAdv(start);
		if (curChr == character)
		{
			return true;
		}
	} while (curChr > 0);

	return false;*/
}

bool TString::Have( const TString& any ) const
{
	byte* start = Data;
	byte* tstart;
	ch32 curChr;
	ch32 curTg;
	do
	{
		curChr = StringDriver::DecodeAdv(start);
		tstart = any.Data;
		do 
		{
			curTg = StringDriver::DecodeAdv(tstart);
			if (curChr == curTg)
			{
				return true;
			}
		} while (curTg > 0);

	} while(curChr > 0);
	return false;
}

TString TString::ToLower() const
{
	TString result (Capacity + (Capacity / 2)); // 1.5x cap
	byte* src = Data;
	byte* end = src + ByteLength;

	TChar curChar;

	while(src <= end)
	{
		curChar = StringDriver::DecodeAdv(src);
		curChar = curChar.ToLower();
		result += curChar;
	}

	return result;
}

TString TString::ToUpper() const
{
	TString result (Capacity + (Capacity / 2)); // 1.5x cap
	byte* src = Data;
	byte* end = src + ByteLength;

	TChar curChar;
	while(src <= end)
	{
		curChar = StringDriver::DecodeAdv(src);
		curChar = curChar.ToUpper();
		result += curChar;
	}

	return result;
}

TString TString::Substring( dword startIndex, dword lengt ) const
{
	if (startIndex + lengt > Length)
	{
		lengt = Length - startIndex;
	}

	if (lengt <= 0)
	{
		return Empty; // string
	}


	byte* cpyStart = Data;

	if (startIndex > 0)
	{
		cpyStart = StringDriver::Count(Data,startIndex); 
	}

	byte* cpyEnd = StringDriver::Count(cpyStart,lengt);
	int copyLength = cpyEnd - cpyStart;

	TString result(copyLength);
	MemoryDriver::Copy(result.Data,cpyStart,copyLength);
	result.ByteLength = copyLength;
	result.Length = lengt;
	result.Data[copyLength] = 0;
	return result;
}

TString TString::Substring( dword startIndex ) const
{
	if (startIndex == 0)
	{
		return *this;
	}

	if (startIndex >= Length)
	{
		return Empty;
	}

	byte* cpyStart = StringDriver::Count(Data,startIndex);
	int skipLength = cpyStart - Data;
	int copyLength = ByteLength - skipLength;

	TString result(copyLength);
	MemoryDriver::Copy(result.Data,cpyStart,copyLength);
	result.ByteLength = copyLength;
	result.Length = Length - startIndex;
	result.Data[copyLength] = 0;
	return result;
}

TArray<TString*> TString::Split( const TArray<ch32>& seprators, bool removeEmpty /*= false*/ ) const
{
	TArray<TString*> result;

	if (seprators.Count == 0)
	{
		result.Add( (TString*)this );
		return result;
	}

	int LastSeprationByteIndex = 0;
	int LastSeprationCharIndex = 0;
	byte* PreviousChar = Data;

	TCharacterEnumerator schars(*this);
	while(schars.MoveNext())
	{
		int k = seprators.Count;
		while(k--)
		{
			if (schars.Current == seprators.Item[k])
			{
				int seprationByteIndex = PreviousChar - Data;
				int seprationLength =  seprationByteIndex - LastSeprationByteIndex;

				if (removeEmpty && seprationLength == 0)
				{
					LastSeprationByteIndex = schars.StrData - Data;
					LastSeprationCharIndex = schars.CharIndex;
				}
				else
				{
					TString* sepration = new TString(Data + LastSeprationByteIndex, seprationLength, (schars.CharIndex - LastSeprationCharIndex)-1);
					result.Add(sepration);
					LastSeprationByteIndex = schars.StrData - Data;
					LastSeprationCharIndex = schars.CharIndex;
				}
				break;
			}
		}
		PreviousChar = schars.StrData;
	}

	if (LastSeprationByteIndex != ByteLength)
	{
		TString* sepration = new TString();
		*sepration = Substring(LastSeprationByteIndex);
		result.Add(sepration);
	}

	return result;
}

TArray<TString*> TString::Split( const TArray<TString*>& seprators, bool removeEMpty /*= false*/ ) const
{
	throw NotImplementedException();
}


int TString::GetTrimStartPoint( const TArray<ch32>& trimChars , int& bytesToSkip)
{
	TCharacterEnumerator schars(*this);
	int trimpoint = 0;
	bool cont;
	byte* lastBytePtr = schars.StrData;
	while(schars.MoveNext())
	{
		int i = trimChars.Count;
		cont = false;
		while(i--)
		{
			if (schars.Current == trimChars.Item[i])
			{
				trimpoint++;
				cont = true;
				break;
			}
		}
		if (!cont)
		{
			break;
		}
		lastBytePtr = schars.StrData;
	}
	bytesToSkip = lastBytePtr - Data;
	return trimpoint;
}

void TString::TrimStartInplace( const TArray<ch32>& trimChars )
{
	int trimBytes;
	int trimpoint = GetTrimStartPoint(trimChars,trimBytes);

	DetachToEdit();
	
	int rembytes = ByteLength - trimBytes;
	int i = 0;
	while(rembytes--)
	{
		Data[i] = Data[i+trimBytes];
		i++;
	}

	ByteLength -= trimBytes;
	Length -= trimpoint;
	Data[ByteLength] = 0;
}

void TString::TrimEndInplace( const TArray<ch32>& trimChars )
{
	int trimBytes;
	int trimpoint = GetTrimEndPoint(trimChars,trimBytes);

	DetachToEdit(trimBytes);

	ByteLength = trimBytes;
	Length = trimpoint;
	Data[ByteLength] = 0;
}

void TString::TrimInplace( const TArray<ch32>& trimChars )
{
	TrimEndInplace(trimChars);
	TrimStartInplace(trimChars);
}


TString TString::TrimStart( const TArray<ch32>& trimChars  )
{
	int trimBytes;
	int trimpoint = GetTrimStartPoint(trimChars,trimBytes);

	if (trimpoint == 0)
	{
		return *this;
	}

	return TString(Data+trimBytes,ByteLength-trimBytes,Length-trimpoint);
}

int TString::GetTrimEndPoint( const TArray<ch32>& trimChars , int& bytesToSkip)
{
	TCharacterReverseEnumerator schars(*this);
	int trimpoint = Length;
	bool cont;
	byte* lastBytePtr = schars.EndData;
	while(schars.MoveNext())
	{
		int i = trimChars.Count;
		cont = false;
		while(i--)
		{
			if (schars.Current == trimChars.Item[i])
			{
				trimpoint--;
				cont = true;
				break;
			}
		}

		if (!cont)
		{
			break;
		}
		lastBytePtr = schars.EndData;
	}

	bytesToSkip = (lastBytePtr - Data) + 1;
	return trimpoint;
}

TString TString::TrimEnd( const TArray<ch32>& trimChars )
{
	int trimBytes;
	int trimpoint = GetTrimEndPoint(trimChars,trimBytes);

	if (trimpoint == Length)
	{
		return *this;
	}

	return TString(Data,trimBytes,trimpoint);
}

void TString::ToLowerInplace()
{
	DetachToEdit();
	byte* src = Data;
	byte* dst = Data;
	byte* end = src + ByteLength;

	TChar curChar;

	while(src <= end)
	{
		curChar = StringDriver::DecodeAdv(src);
		curChar = curChar.ToLower();
		StringDriver::EncodeAdv(dst,curChar.Character);
	}
}

void TString::ToUpperInplace()
{
	throw NotImplementedException();
}



/*TString TString::Format( const TString& format,... )
{
	va_list args;
	va_start (args, format);

	byte* start = format.Data;
	ch32 curChr;

	do 
	{
		curChr = StringDriver::DecodeAdv(start);
		if (curChr == '{')
		{
			ch32 cur = StringDriver::DecodeAdv(start);
			if (curChr >= '0' && curChr <= '9' )
			{
				curChr = StringDriver::DecodeAdv(start);
				if ()
				{

				}
			}
		}
	} while (curChr > 0);

}*/