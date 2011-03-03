#ifndef TSTREAMWRITER_H
#define TSTREAMWRITER_H

#include "tbufferedstream.h"
#include "ttextwriter.h"
#include "tfilestream.h"

class TStreamWriter: public TTextWriter
{
public:
	TStream* BaseStream;
	TBufferedStream* BufSource; // Buffered Source

	TStreamWriter(TStream* stream,int bufferSize = 8192)
	{
		BaseStream = stream;
		BufSource = new TBufferedStream(stream,bufferSize);
		NewLine = &CrLf8;
		Encoding = TEncoding::ASCII;
	}

	TStreamWriter(const str8& path,int bufferSize = 8192) // open file
	{
		TStream* srcStream = TFileStream::Open(path,fm_Write);
		BaseStream = srcStream;
		BufSource = new TBufferedStream(srcStream,bufferSize);
		NewLine = &CrLf8;
		Encoding = TEncoding::ASCII;
	}

	inline void Write(const str8& value)
	{
		// TODO: fix this shit with encoding converter
		if (Encoding == TEncoding::ASCII || Encoding == TEncoding::UTF8)
		{
			BufSource->Write(value.Chars,1,value.Length);
		}
		else
		{
			str16 tmpStr;
			tmpStr = value;
			Write(tmpStr);
		}
	}

	void Write(const str16& value)
	{
		if (Encoding == TEncoding::Unicode)
		{
			BufSource->Write(value.Chars,2,value.Length);
		}
		else
		{
			str8 tmpStr;
			tmpStr = value;
			Write(tmpStr);
		}
	}

	void Close()
	{
		BufSource->Close();
		delete BufSource;
		BufSource = 0;
	}
};

#endif