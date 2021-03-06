#ifndef TBINARYWRITER_H
#define TBINARYWRITER_H

#include "tstream.h"
#include "tarray.h"
#include "tfilestream.h"

/**
 * BinaryWriter template
 * It implemented as template so it can be used in other classes with inline high performance?
 */
template <class T>
class TBinaryWriterTemplate
{
public:
	T* BaseStream;

	TBinaryWriterTemplate( T* _BaseStream): BaseStream(_BaseStream)
	{

	}

	inline void Close()
	{
		BaseStream->Close();
	}

	inline void WriteByte(byte value)
	{
		BaseStream->Write(&value,1,1);
	}

	inline void WriteByteArray(byte* value, int length)
	{
		BaseStream->Write(value,1,length);
	}

	inline void WriteByteArray(Array<byte>& value)
	{
		WriteByteArray(value.Items,value.Count);
	}

	inline void WriteDecimal()
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	inline void WriteDouble(double value)
	{
		BaseStream->Write(&value,1,sizeof(value));
	}

	inline void WriteInt16(i16 value)
	{
		BaseStream->Write(&value,1,sizeof(value));
	}

	inline void WriteInt32(i32 value)
	{
		BaseStream->Write(&value,1,sizeof(value));
	}

	inline void WriteInt64(i64 value)
	{
		BaseStream->Write(&value,1,sizeof(value));
	}

	inline void WriteFloat(float value)
	{
		BaseStream->Write(&value,1,sizeof(value));
	}

	inline void WriteString(const String& value)
	{
		WriteUInt32(value.ByteLength);
		WriteByteArray(value.Data,value.ByteLength);	
	}

	inline void WriteUInt16(ui16 value)
	{
		BaseStream->Write(&value,1,sizeof(value));
	}

	inline void WriteUInt32(ui32 value)
	{
		BaseStream->Write(&value,1,sizeof(value));
	}

	inline void WriteUInt64(ui64 value)
	{
		BaseStream->Write(&value,1,sizeof(value));
	}
};

typedef TBinaryWriterTemplate<TStream> TBinaryWriter;
typedef TBinaryWriterTemplate<TFileStream> TBinaryWriterFile;

#endif