#ifndef TSTRINGREADER_H
#define TSTRINGREADER_H

#include "ttextreader.h"
#include "tmemorydriver.h"

class TStringReader: public TTextReader
{
public:

	String SourceString;
	int CurrentIndex;
	int CurrentByteIndex;

	TStringReader()
	{
		CurrentIndex = 0;
		CurrentByteIndex = 0;
		EndOfStream = false;
	}

	TStringReader(const String& srcString)
	{
		Initialize(srcString);
	}

	void Initialize(const String& srcString)
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

	String ReadLine() 
	{
		int interrupt;
		String result = ReadUntil("\n","\r", interrupt);
		return result;
	}

	String ReadToEnd() 
	{
		return SourceString;
	}

	String ReadUntil( const String& interrupChars, const String& ignoreChars, int& interrupt ) 
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

	String ReadUntilString(const String& matchString) 
	{
		TStringBuilder sb;
		ch32 firstChar = matchString.GetFirst();

		int lastIndex;
		int lastByteIndex;

		while (!EndOfStream)
		{
			lastIndex = CurrentIndex;
			lastByteIndex = CurrentByteIndex;

			ch32 curChar = Read();
			if (curChar == firstChar)
			{
				if (SourceString.ByteLength - lastByteIndex >= matchString.ByteLength)
				{
					int result = MemoryDriver::Compare(SourceString.Data+lastByteIndex, matchString.Data, matchString.ByteLength);
					if (result == 0)
					{
						CurrentIndex = lastIndex + matchString.Length;
						CurrentByteIndex = lastByteIndex + matchString.ByteLength;
						if (CurrentByteIndex == SourceString.ByteLength)
						{
							EndOfStream = true;
						}
						return sb.ToString();
					}
					else
					{
						CurrentIndex = lastIndex;
						CurrentByteIndex = lastByteIndex;
						curChar = Read();
					}
				}
			}

			sb.AppendUnicode(curChar);
		}

		return sb.ToString();
	}

	void Close() 
	{
		delete this;
	}
};

#endif // !TSTRINGREADER_H
