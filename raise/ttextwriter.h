#ifndef TTEXTWRITER_H
#define TTEXTWRITER_H

#include "tstring.h"
#include "tstringbuilder.h"
#include "tencoding.h"

//TODO: make this class converts its content to UTF-8 to UTF-16

class TTextWriter
{
public:
	TEncoding& Encoding;

	TTextWriter(): Encoding( (TEncoding&)TEncoding::UTF8 ) { };

	virtual void Write(const TString& value) = 0;

	virtual void Write(const TStringBuilder& builder) = 0;

	virtual void WriteLine() = 0;

	virtual void WriteLine(const TString& value)
	{
		Write(value);
		WriteLine();
	}

	virtual void SetNewLine(const TString& value) = 0;

	virtual void Close() = 0;

protected:
	byte NewLineBytes[16];
	int NewLineByteCount;
};



#endif