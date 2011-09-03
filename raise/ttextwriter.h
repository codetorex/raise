#ifndef TTEXTWRITER_H
#define TTEXTWRITER_H

#include "tstring.h"
#include "tencoding.h"

//TODO: make this class converts its content to UTF-8 to UTF-16

class TTextWriter
{
private:
	inline void WriteNewLine()
	{
		
	}

	byte NewLineBytes[16];
	int NewLineByteCount;

public:
	TEncoding* Encoding;

	virtual void Write(const TString& value) = 0;

	virtual void WriteLine(const TString& value)
	{
		Write(value);
		WriteNewLine();
	}

	void SetNewLine(const TString& value)
	{

	}

	virtual void Close() = 0;
};



#endif