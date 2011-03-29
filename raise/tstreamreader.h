#ifndef TSTREAMREADER_H
#define TSTREAMREADER_H

#include "tbufferedstream.h"
#include "ttextreader.h"
#include "tfilestream.h"


class TStreamReader: public TTextReader
{
public:
	TStream* BaseStream;
	TBufferedStream* BufSource; // Buffered Source

	bool EndOfStream;

	TStreamReader(TStream* stream,int bufferSize = 8192)
	{
		BaseStream = stream;
		BufSource = new TBufferedStream(stream,bufferSize);
		EndOfStream = false;
	}

	TStreamReader(const str8& path,int bufferSize = 8192) // open file
	{
		TStream* srcStream = TFileStream::Open(path,fm_Read);
		BaseStream = srcStream;
		BufSource = new TBufferedStream(srcStream,bufferSize);
		EndOfStream = false;
	}

	int Peek()
	{
		return BufSource->Peek(); // TODO: This function should read as char.
	}

	int Read()
	{
		return BufSource->ReadByte(); // TODO: This function should read as char.
	}

	int Read(ch16* buffer,int count)
	{
		throw Exception("Not implemented");
		return 0;
		// TODO: This function should read as char.
	}

	str8 ReadLine()
	{
		str8 result(1024); // TODO: This function should read as char. check UTF compatability of document and there should be string encoding system defined as in .net framewrok
		while(1)
		{
			int d = BufSource->ReadByte();
			if (d < 0)
			{
				EndOfStream = true;
				break;
			}

			if (d != 0x0A)
			{
				if (d != 0x0D)
				{
					result += ((ch8)d);
				}
			}
			else
			{
				break;
			}
		}

		return result;
	}

	str8 ReadToEnd()
	{
		// TODO: This function should read as char.
		int length = BaseStream->Length();
		BaseStream->Seek(0,sBegin);
		str8 result(length);

		int d = BufSource->ReadByte();
		while(d > 0)
		{
			result += ((ch16)d);
			d = BufSource->ReadByte();
		}

		EndOfStream = true;

		return result;
	}

	/**
	* Closes base stream and commits suicide.
	*/
	void Close()
	{
		BufSource->Close();
		BufSource = 0;
		delete this;
	}

	//virtual void DiscardBufferedData() = 0;
};


#endif

