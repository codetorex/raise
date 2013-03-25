#ifndef TSTRINGREADER_H
#define TSTRINGREADER_H

#include "ttextreader.h"

class TStringReader: public TTextReader
{
public:

	TString SourceString;
	int CurrentIndex;
	int CurrentByteIndex;

	TStringReader()
	{
		CurrentIndex = 0;
		CurrentByteIndex = 0;
		EndOfStream = false;
	}

	TStringReader(const TString& srcString)
	{
		Initialize(srcString);
	}

	void Initialize(const TString& srcString)
	{
		SourceString = srcString;
		CurrentIndex = 0;
		CurrentByteIndex = 0;
		EndOfStream = false;
	}

	void Rewind()
	{
		CurrentByteIndex = 0;
		CurrentIndex = 0;
	}

	ch32 Peek() 
	{
		if (EndOfStream)
		{
			return -1;
		}

		int byteLength;
		ch32 curChar = StringDriver::Decode(SourceString.Data + CurrentByteIndex, byteLength);
		return curChar;
	}

	ch32 Read() 
	{
		if (EndOfStream)
		{
			return -1;
		}
		
		int byteLength;
		ch32 curChar = StringDriver::Decode(SourceString.Data + CurrentByteIndex, byteLength);
		CurrentIndex++;
		CurrentByteIndex += byteLength;
		if (CurrentByteIndex == SourceString.ByteLength)
		{
			EndOfStream = true;
		}
		return curChar;
	}

	int Read( ch32* buffer, int count ) 
	{
		int i=0;
		while (count-- > 0)
		{
			ch32 curChar = Read();
			if (curChar == -1)
			{
				return i;
			}
			buffer[i++] = curChar;
		}

		return i;
	}

	TString ReadLine() 
	{
		int interrupt;
		TString result = ReadUntil("\n","\r", interrupt);
		return result;
	}

	TString ReadToEnd() 
	{
		return SourceString;
	}

	TString ReadUntil( const TString& interrupChars, const TString& ignoreChars, int& interrupt ) 
	{
		TStringBuilder result;

		while (!EndOfStream)
		{
			ch32 curChar = Read();
			
			if (interrupChars.Have(curChar))
			{
				interrupt = curChar;
				return result.ToString();
			}
			if (!ignoreChars.Have(curChar))
			{
				result.AppendUnicode(curChar);
			}
		}

		interrupt = -1;
		return result.ToString();
	}

	TString ReadUntilString(const TString& matchString) 
	{
		TStringBuilder sb;
		ch32 firstChar = matchString.GetFirst();

		int lastIndex;
		int lastByteIndex;

		while (!EndOfStream)
		{
			ch32 curChar = Read();
			if (curChar == firstChar)
			{
				lastIndex = CurrentIndex;
				lastByteIndex = CurrentByteIndex;

				bool match = true;
				TCharacterEnumerator ce(matchString);
				while (ce.MoveNext())
				{
					if (curChar != ce.Current)
					{
						match = false;
					}
					curChar = Read();
				}

				if (match)
				{
					return sb.ToString();
				}
				else
				{
					CurrentIndex = lastIndex;
					CurrentByteIndex = lastByteIndex;
				}
			}

			sb.AppendUnicode(curChar);
		}
	}

	void Close() 
	{
		delete this;
	}
};

#endif // !TSTRINGREADER_H
