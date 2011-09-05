#ifndef TSTREAMWRITER_H
#define TSTREAMWRITER_H

#include "tbufferedstream.h"
#include "ttextwriter.h"
#include "tfile.h"

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
		Encoding = TEncodingInfo::ASCII;
	}

	TStreamWriter(const TString& path,int bufferSize = 8192) // open file
	{
		TStream* srcStream = File::OpenWrite(path);
		BaseStream = srcStream;
		BufSource = new TBufferedStream(srcStream,bufferSize);
		NewLine = &CrLf8;
		Encoding = TEncodingInfo::ASCII;
	}

	inline void Write(const TString& value)
	{
		// TODO: fix this shit with encoding converter
		if (Encoding == TEncodingInfo::ASCII || Encoding == TEncodingInfo::UTF8)
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

	void Close()
	{
		BufSource->Close();
		delete BufSource;
		BufSource = 0;
	}
};

#endif