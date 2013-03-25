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
	}

	void Rewind()
	{
		CurrentByteIndex = 0;
		CurrentIndex = 0;
	}

	ch32 Peek() 
	{
		if (CurrentByteIndex == SourceString.ByteLength)
		{
			return -1;
		}

		int byteLength;
		ch32 curChar = StringDriver::Decode(SourceString.Data + CurrentByteIndex, byteLength);
		return curChar;
	}

	ch32 Read() 
	{
		if (CurrentByteIndex == SourceString.ByteLength)
		{
			return -1; // no more bytes left to read
		}
		int byteLength;
		ch32 curChar = StringDriver::Decode(SourceString.Data + CurrentByteIndex, byteLength);
		CurrentIndex++;
		CurrentByteIndex += byteLength;
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
		TString result = ReadUntil("\r\n",TString::Empty, interrupt);
		ch32 nextChar = Peek();
		if (nextChar == '\r' || nextChar == '\n')
		{
			Read(); // skip single char
		}
		return result;
	}

	TString ReadToEnd() 
	{
		return SourceString;
	}

	TString ReadUntil( const TString& interrupChars, const TString& ignoreChars, int& interrupt ) 
	{
		TStringBuilder result;

		while (true)
		{
			ch32 curChar = Read();
			if (curChar == -1)
			{
				interrupt = -1;
				return result.ToString();
			}
			else if (interrupChars.Have(curChar))
			{
				interrupt = curChar;
				return result.ToString();
			}

			result.AppendUnicode(curChar);
		}
	}

	void Close() 
	{
		delete this;
	}
};

#endif // !TSTRINGREADER_H
