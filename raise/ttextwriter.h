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
		if (Encoding == TEncodingInfo::ASCII || Encoding == TEncodingInfo::UTF8)
		{
			Write(*NewLine);
		}
		else if (Encoding == TEncodingInfo::Unicode)
		{
			Write(*NewLine16);
		}
	}

public:
	str8* NewLine;
	str16* NewLine16;
	// TODO: write encoding class and fix this shit
	TEncodingInfo* Encoding;

	virtual void Write(const str8& value) = 0;
	virtual void Write(const str16& value) = 0;

	virtual void WriteLine(const str8& value)
	{
		Write(value);
		WriteNewLine();
	}

	virtual void WriteLine(const str16& value)
	{
		Write(value);
		WriteNewLine();
	}

	virtual void Close() = 0;
};



#endif