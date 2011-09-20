#ifndef TBINARYREADER_H
#define TBINARYREADER_H

#include "tbufferedstream.h"

class TBinaryReader
{
public:
	TStream* BaseStream;
	//TBufferedStream* BufSource; // Buffered Source
	TStream* BufSource; // not using buffer this time

	bool EndOfStream;

	TBinaryReader(TStream* stream,int bufferSize = 8192)
	{
		BaseStream = stream;
		//BufSource = new TBufferedStream(stream,bufferSize);
		BufSource = stream;
		EndOfStream = false;
	}

	int Read(byte* buffer,int size,int count)
	{
		return BufSource->Read(buffer,size,count);
	}

	/**
	* Reads single byte.
	* Gives exception if end of stream.
	* @return value readed
	*/
	inline byte ReadByte()
	{
		return (byte)BufSource->ReadByte();
	}

	/**
	* Reads a boolean value.
	* Gives exception if end of stream.
	* @return bool readed
	*/
	inline bool ReadBoolean()
	{
		byte booldata = ReadByte();
		return (booldata > 0 ? true : false);
	}

	/**
	* Reads some bytes from stream.
	* @return Pointer to readed bytes, 0 if end of stream.
	*/
	inline byte* ReadBytes(int count)
	{
		byte* result = new byte [count];
		int readed = Read(result,1,count);
		if (readed == 0)
		{
			delete [] result;
			EndOfStream = true;
			return 0;
		}
		return result;
	}

	inline uint16 ReadWord()
	{
		uint16 result;
		Read((byte*)&result,2,1);
		return result;
	}

	inline ui32 ReadDWord()
	{
		ui32 result;
		Read((byte*)&result,4,1);
		return result;
	}

	inline int ReadInt()
	{
		int result;
		Read((byte*)&result,4,1);
		return result;
	}

	inline short ReadShort()
	{
		short result;
		Read((byte*)&result,2,1);
		return result;
	}

	inline float ReadFloat()
	{
		float result;
		Read((byte*)&result,4,1);
		return result;
	}

	inline double ReadDouble()
	{
		double result;
		Read((byte*)&result,8,1);
		return result;
	}

	// TODO: write 64-bit reading

	/**
	* Closes base stream and commits suicide.
	*/
	void Close()
	{
		BufSource->Close();
		BufSource = 0;
		delete this;
	}
};

#endif