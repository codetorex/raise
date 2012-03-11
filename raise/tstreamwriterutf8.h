#ifndef TSTREAMWRITERUTF8_H
#define TSTREAMWRITERUTF8_H

#include "tstream.h"
#include "ttextwriter.h"

/**
 * UTF8 optimized stream text writer.
 * TODO: no buffered stream introduced but it will be good?
 */
class TStreamWriterUTF8: public TTextWriter
{
public:
	TStream* BaseStream;

	TStreamWriterUTF8()
	{
		Encoding = (TEncoding&)TEncoding::UTF8;
	}

	inline void Initialize(TStream* output)
	{
		BaseStream = output;
		BaseStream->Write(UTF8ByteOrderMark,1,UTF8ByteOrderMarkLength); // WRITE BOM
	}

	TStreamWriterUTF8(TStream* output)
	{
		Initialize(output);
	}

	inline void WriteChar(ch32 chr)
	{
		byte encoded[8];
		int len = StringDriver::Encode(encoded,chr);
		BaseStream->Write(encoded,1,len);
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
		Write(NewLine);
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