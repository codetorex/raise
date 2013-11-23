#include "stdafx.h"
#include "tsoundwav.h"

// TODO: abstractize these stuff
#include "tbinaryreader.h"
#include "tbinarywriter.h"

ui32 StringAsUInt32(const TString& input)
{
	if (input.ByteLength > 4)
	{
		throw Exception("Unsupported format");
	}
	return *(ui32*)input.Data;
}

struct RIFFData
{
	ui32 Format;
};

struct FMTData
{
	ui16 AudioFormat;
	ui16 NumChannels;
	ui32 SampleRate;
	ui32 ByteRate;
	ui16 BlockAlign;
	ui16 BitsPerSample;
};

class RIFFChunk
{
public:
	ui32 ChunkID;
	ui32 ChunkSize;
	byte* ChunkData;

	void SetChunkID(const TString& chunkId)
	{
		ChunkID = StringAsUInt32(chunkId);
	}

	void ReadChunk(TBinaryReader* br)
	{
		ChunkID = br->ReadUInt32();
		ChunkSize = br->ReadUInt32();
		ChunkData = br->ReadBytes(ChunkSize);
	}

	void WriteChunk(TBinaryWriter* bw)
	{
		bw->WriteUInt32(ChunkID);
		bw->WriteUInt32(ChunkSize);
		bw->WriteByteArray(ChunkData, ChunkSize);
	}

	template<class T>
	T* GetData()
	{
		return (T*)ChunkData;
	}

	template<class T>
	void SetData(T* d)
	{
		ChunkData = (byte*)d;
	}
};

void TSoundWAV::ReadSound( TSound* snd, Stream* src )
{
	TBinaryReader* br = new TBinaryReader(src);

	RIFFChunk header;
	header.ReadChunk(br);

	RIFFData* rd  = header.GetData<RIFFData>();
	
}

void TSoundWAV::WriteSound( TSound* snd, Stream* dst )
{
	TBinaryWriter* bw = new TBinaryWriter(dst);

	RIFFChunk header;
	header.SetChunkID("RIFF");
	header.ChunkSize = 4;
	
	RIFFData data;
	data.Format = StringAsUInt32("WAVE");
	header.SetData(&data);

	header.WriteChunk(bw);

	RIFFChunk format;
	format.SetChunkID("fmt ");


}
