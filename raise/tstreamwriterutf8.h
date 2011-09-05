#ifndef TSTREAMWRITERUTF8_H
#define TSTREAMWRITERUTF8_H

#include "tstream.h"
#include "ttextwriter.h"

class TStreamWriterUTF8: public TTextWriter
{
public:
	TStream* BaseStream;

	TStreamWriterUTF8(TStream* output)
	{
		BaseStream = output;
		BaseStream->Write(UTF8ByteOrderMark,1,UTF8ByteOrderMarkLength); // WRITE BOM
		Encoding = (TEncoding&)TEncoding::UTF8;
	}

	inline void Write(const TString& value)
	{
		BaseStream->Write(value.Data,1,value.ByteLength);
	}

	inline void Write(const TStringBuilder& builder)
	{
		BaseStream->Write(builder.GetData(),1,builder.ByteLength);
	}

	inline void WriteLine()
	{
		BaseStream->Write(NewLineBytes,1,NewLineByteCount);
	}

	void SetNewLine(const TString& value)
	{
		if (value.ByteLength > 16)
		{
			throw Exception("Wrong parameter");
		}
		value.CopyTo(NewLineBytes,value.ByteLength);
		NewLineByteCount = value.ByteLength;	
	}

	void Close()
	{
		BaseStream->Close();
		delete this;
	}

private:
	static byte UTF8ByteOrderMark[];
	static int UTF8ByteOrderMarkLength;
};

#endif