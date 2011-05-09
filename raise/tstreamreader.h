#ifndef TSTREAMREADER_H
#define TSTREAMREADER_H

#include "tbufferedstream.h"
#include "tmemorystream.h"
#include "ttextreader.h"
#include "tfile.h"



template <class T>
class TStreamReader: public TTextReader
{
private:
	void LoadStream(TStream* stream,int bufferSize);

public:
	/// The base stream
	TStream* BaseStream;

	/// The buffer source
	T* BufSource;


	/**
	 * @brief Constructor with stream.
	 * @param [in] stream The stream to be readed.
	 * @param bufferSize Size of the buffer.
	 */
	TStreamReader(TStream* stream,int bufferSize = 8192)
	{
		LoadStream(stream,bufferSize);
	}


	/**
	 * @brief Constructor with filename.
	 * @param path Full pathname of the file.
	 * @param bufferSize Size of the buffer.
	 */
	TStreamReader(const str8& path,int bufferSize = 8192) // open file
	{
		LoadStream(File::OpenRead(path),bufferSize);
	}

	inline ch32 Peek()
	{
		return BufSource->PeekByte(); // TODO: This function should read as char.
	}

	inline ch32 Read()
	{
		return BufSource->ReadByte(); // TODO: This function should read as char.
	}

	inline int Read(ch16* buffer,int count)
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
				else
				{
					break;
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

	str8 ReadInterrupted(const str8& interrupChars, const str8& ignoreChars, int& interrupt)
	{
		str8 readed(512);
		int d = BufSource->ReadByte();
		while(d > 0)
		{
			if (interrupChars.HaveChar(d))
			{
				interrupt = d;
				return readed;
			}
			if (!ignoreChars.HaveChar(d))
			{
				readed += ((ch8)d);
			}
			d = BufSource->ReadByte();
		}

		EndOfStream = true;
		interrupt = -1;
		return readed;
	}

	/**
	 * @brief Closes stream and commits suicide.
	 */
	void Close()
	{
		BufSource->Close();
		BufSource = 0;
		delete this;
	}

	//virtual void DiscardBufferedData() = 0;
};

/**
* Uses buffered stream and reads from real file with some buffer.
*/
typedef TStreamReader<TBufferedStream>	StreamReader;

/**
* Faster version. Which uses memory cached stream as source. Not recommended for big files. But what the hell...
*/
typedef TStreamReader<TMemoryStream>	CacheReader;

#endif

