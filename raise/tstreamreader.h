#ifndef TSTREAMREADER_H
#define TSTREAMREADER_H

#include "tbufferedstream.h"
#include "tmemorystream.h"
#include "ttextreader.h"
#include "tfile.h"
#include "tencoding.h"


//TODO: bufferi char buffer olarak yap süper olur .net stayla, boylece TBufferedStreame felan gerek kalmaz.
// yani catir catir okuyup ch32 olarak bufferda tutsun, bunun icin charBuffer gibi bir class yapabiliriz

class TStreamReader: public TTextReader
{
private:

	void DetectEncodingFromBOM(const TEncoding& defaultencoding)
	{
		BaseStream->Read(Cache.Data,1,4);
		ui16 utf16char = ((ui16*)Cache.Data)[0];
		ui32 utf32char = ((ui32*)Cache.Data)[0];

		if (Cache.Data[0] == 0xEF)
		{
			if (Cache.Data[1] == 0xBB)
			{
				if (Cache.Data[2] == 0xBF)
				{
					CurrentEncoding = (TEncoding&)TEncoding::UTF8;
					Cache.Data[0] = Cache.Data[3];
					CacheStart = 1;
				}
			}
		}
		else if ( utf16char == 0xFEFF )
		{
			CurrentEncoding = (TEncoding&)TEncoding::UTF16;
			Cache.Data[0] = Cache.Data[2];
			Cache.Data[1] = Cache.Data[3];
			CacheStart = 2;
		}
		else if (utf16char == 0xFFFE )
		{
			throw NotImplementedException(); // BIG ENDIAN UTF16
		}
		else if (utf32char == 0xFEFF)
		{
			throw NotImplementedException(); // UTF32 encoding
		}
		else if (utf32char == 0xFFFE0000)
		{
			throw NotImplementedException(); // big endian UTF32 encoding
		}
		else
		{
			CurrentEncoding = defaultencoding;
			CacheStart = 4;
		}
	}

	void LoadStream(TStream* stream,const TEncoding& encoding, bool detectBOM, int bufferSize)
	{
		BaseStream = stream;
		Buffer.InitializeArray( bufferSize );
		EndOfStream = false;
		Cache.Allocate(16384);
		CacheStart = 0;
		BufferReadIndex = 0;
		EndOfBaseStream = false;
		CurrentEncoding = encoding;

		if (detectBOM)
		{
			DetectEncodingFromBOM(encoding);
		}
	}

public:
	/// The base stream
	TStream* BaseStream;

	bool EndOfBaseStream;

	/// Current encoding of stream;
	TEncoding& CurrentEncoding;

	/// The buffer source
	TCharBuffer Buffer;

	TByteArray Cache;

	/// When first readed chunk doesn't completes last character, it postponed to next chunk. So this member will show how much bytes postponed.
	ui32 CacheStart;

	ui32 BufferReadIndex;

	void FillBuffer()
	{
		if (EndOfBaseStream) return;

		Buffer.Rewind();
		ui32 readed = BaseStream->Read( Cache.Data + CacheStart, 1, Cache.Capacity - CacheStart);
		bool isEnded = readed < Cache.Capacity - CacheStart;
		ui32 used = CurrentEncoding.GetChars(Cache.Data,readed + CacheStart,Buffer);
	

		if (used < readed)
		{
			CacheStart = readed - used;
			MemoryDriver::Copy(Cache.Data,Cache.Data + used, CacheStart);
		}
		else
		{
			CacheStart = 0;
		}

		BufferReadIndex = 0;

		if( isEnded )
		{
			EndOfBaseStream = true;
			Buffer.AddCharacter( MXDWORD );
		}
	}

	/**
	 * @brief Constructor with stream.
	 * @param [in] stream The stream to be readed.
	 * @param bufferSize Size of the buffer.
	 */
	TStreamReader(TStream* stream,int bufferSize = 8192) : CurrentEncoding( (TEncoding&)TEncoding::Latin1 ) 
	{
		LoadStream(stream,CurrentEncoding,true,bufferSize);
	}

	TStreamReader(TStream* stream,const TEncoding& encoding, bool detectBOM = true, int buffersize = 8192): CurrentEncoding( (TEncoding&)TEncoding::Latin1 ) 
	{
		LoadStream(stream,encoding,detectBOM,buffersize);
	}

	/**
	 * @brief Constructor with filename.
	 * @param path Full pathname of the file.
	 * @param bufferSize Size of the buffer.
	 */
	TStreamReader(const TString& path,int bufferSize = 8192): CurrentEncoding( (TEncoding&)TEncoding::Latin1 ) 
	{
		LoadStream(File::OpenRead(path),CurrentEncoding,true,bufferSize);
	}

	inline ch32 Peek()
	{
		if ( BufferReadIndex == Buffer.Index )
		{
			if (!EndOfBaseStream)
			{
				FillBuffer();
			}
			else
			{
				EndOfStream = true;
				return MXDWORD;
			}
		}
		return Buffer.Data[BufferReadIndex];
	}

	inline ch32 Read()
	{
		if ( BufferReadIndex == Buffer.Index )
		{
			if (!EndOfBaseStream)
			{
				FillBuffer();
			}
			else
			{
				EndOfStream = true;
				return MXDWORD;
			}
		}
		return Buffer.Data[BufferReadIndex++];
	}

	inline int Read(ch32* buffer,int count)
	{
		throw NotImplementedException();
		// TODO: This function should read as char.
	}

	TString ReadLine()
	{
		TString result(1024); 
		ch32 d;

		int i;
		while(1)
		{
			d = Read();
			if (EndOfStream)
				break;

			if (d != 0x0A)
			{
				if (d != 0x0D)
				{
					result.AppendUnicodeFast(d);
					i = result.Capacity - result.ByteLength;
					if (i < 8)
					{
						result.IncreaseCapacity();
					}
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

		// skip end of line character
		d = Peek();
		if (d == 0x0A || d == 0x0D)
		{
			Read();
		}
		
		result.Data[result.ByteLength] = 0;
		
		return result;
	}

	TString ReadToEnd()
	{
		// TODO: this function can be optimized but who cares?
		TString result(1024);

		ch32 d = Read();
		while(d > 0)
		{
			result += d;
			d = Read();
		}

		EndOfStream = true;

		return result;
	}

	TString ReadInterrupted(const TString& interrupChars, const TString& ignoreChars, int& interrupt)
	{
		TString readed(512);
		ch32 d = Read();
		while(!EndOfStream)
		{
			if (interrupChars.Have(d))
			{
				interrupt = d;
				return readed;
			}
			if (!ignoreChars.Have(d))
			{
				readed += d;
			}
			d = Read();
		}

		interrupt = -1;
		return readed;
	}

	TString ReadWord(const TString& trimCharacters)
	{
		TString readed(512);
		ch32 d = Read();

		int mode = 0;

		while(!EndOfStream)
		{
			if (trimCharacters.Have(d))
			{
				if (mode == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}

			mode = 1;
			readed += d;
			d = Read();
		}

		return readed;
	}

	//void SkipUntil(const TString& )

	/**
	 * @brief Closes stream and commits suicide.
	 */
	void Close()
	{
		BaseStream->Close();
		BaseStream = 0;
		delete this;
	}

	//virtual void DiscardBufferedData() = 0;
};

#endif

