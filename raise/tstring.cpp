#include "stdafx.h"
#include "tstring.h"
#include "tutf16encoding.h"
#include "tarray.h"

#include "tstringformat.h"

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

TString TString::Substring( ui32 startIndex, ui32 lengt ) const
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

TString TString::Substring( ui32 startIndex ) const
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
					LastSeprationCharIndex = schars.CharIndex+1;
				}
				else
				{
					TString* sepration = new TString(Data + LastSeprationByteIndex, seprationLength, (schars.CharIndex - LastSeprationCharIndex));
					result.Add(sepration);
					LastSeprationByteIndex = schars.StrData - Data;
					LastSeprationCharIndex = schars.CharIndex+1;
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

TArray<TString*> TString::Split( const TArray<TString*>& seprators, bool removeEmpty /*= false*/ ) const
{
	throw NotImplementedException(__FILE__,__LINE__);
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
	DetachToEdit();
	byte* src = Data;
	byte* dst = Data;
	byte* end = src + ByteLength;

	TChar curChar;

	while(src <= end)
	{
		curChar = StringDriver::DecodeAdv(src);
		curChar = curChar.ToUpper();
		StringDriver::EncodeAdv(dst,curChar.Character);
	}
}

TString TString::Format( const TString& format,int argc , const TStringFormatElementBase** args )
{
	TStringBuilderStack<512> sb;

	int curarg = 0;
	for (ui32 i=0;i<format.ByteLength;i++)
	{
		char curChar = format.Data[i];
		if (curChar == '%')
		{
			if (format.Data[i+1] == '%')
			{
				sb.AppendCharFaster('%');
				i++; // skip next char
			}
			else
			{
				if (curarg == argc)
				{
					// NOTE FROM BACK OF TIME:
					// SOME PEOPLE MAY USE THIS FOR CRASHING SERVER OR SOMETHING
					// BE CARFUL WHILE PROCESSING USER DATA.
					throw Exception("Format argument count mismatch");
				}
				const TStringFormatElementBase* fmtbase = args[curarg++];
				sb.Append(*fmtbase);
			}
		}
		else
		{
			sb.AppendCharFaster(curChar);
		}
	}

	if (format.IsASCII())
	{
		sb.Length = sb.ByteLength;
		sb.Data[sb.ByteLength] = 0;
	}
	else
	{
		sb.Data[sb.ByteLength] = 0;
		sb.ReCount();
	}

	
	return sb.ToString();
}

TString TString::SubstringUntil( ui32 startIndex, ch32 matchCharacter ) const
{
	if (!IsASCII())
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}


}

ui32 TString::SubstringUntil( ui32 startIndex, ch32 matchCharacter, TArray<byte>& output ) const
{
	throw NotImplementedException(__FILE__,__LINE__);
}

TArray<TString*> TString::SplitInplace( TChar character, bool removeEmpty /*= false*/ )
{
	TArray<TString*> result;

	if (ByteLength != Length)
	{
		// UNICODE version of this function is not implemented atm
		throw NotImplementedException(__FILE__,__LINE__);
	}

	int lastWordStart = 0;
	for (int i=0;i<ByteLength;i++)
	{
		char curChar = Data[i];
		if (curChar == character)
		{
			int curWordLength = i-lastWordStart;
			if (curWordLength > 0 )
			{
				result.Add( new TString(Data+lastWordStart, curWordLength,curWordLength));
			}
			else
			{
				if (!removeEmpty)
				{
					result.Add( &TString::Empty );
				}
			}
			Data[i] = 0;
			lastWordStart = i+1;
		}
	}

	return result;
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