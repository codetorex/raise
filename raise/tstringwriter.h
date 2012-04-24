#ifndef TSTRINGWRITER_H
#define TSTRINGWRITER_H

#include "ttextwriter.h"

class TStringWriter: public TTextWriter
{
public:
	
	TStringBuilder sb;

	inline void WriteChar(ch32 chr)
	{
		sb.AppendUnicode(chr);
	}

	inline void Write(const TString& value)
	{
		sb.Append(value);
	}

	inline void Write(const TStringBuilder& builder)
	{
		sb.InternalAppend(builder.Data,builder.ByteLength,builder.Length);
	}

	TString ToString()
	{
		return sb.ToString();
	}
};

#endif