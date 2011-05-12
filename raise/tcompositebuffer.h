#ifndef TFLEXIBLEBUFFER_H
#define TFLEXIBLEBUFFER_H

#include "texception.h" // includes string too
#include "tcomposition.h"
#include "tbuffer.h"

/**
* Buffer that is flexible in format definition.
*/
class TCompositeBuffer: public TBuffer
{
public:
	int CapacityItem;		// Buffer capacity in item count
	int Used;			// Buffer usage in item count

	TBufferFormat* BufferFormat;	// Format definition

	void Free()
	{
		if (Data)
		{
			delete [] Data;
			Data = 0;
		}
		Index = 0;
		CapacityItem = Used = 0;
	}

	void AllocateItemCapacity(int _newItemCapacity);

	void GrowItemCapacity(int _newItemCapacity);

	void AllocateItemCapacity(TBufferFormat* _format = 0,int _itemCapacity = 0);

	inline byte* GetBufferEnd()
	{
		return (Data + Capacity);
	}

	TCompositeBuffer()
	{
		AllocateItemCapacity();
	}

	TCompositeBuffer(TBufferFormat* _format,int _itemCapacity)
	{
		AllocateItemCapacity(_format,_itemCapacity);
	}

	TCompositeBuffer(TBufferFormat* _format,byte* _buffer,int _bufferLength)
	{
		Data = Data;
		Index = _bufferLength;
		Used = CapacityItem = _bufferLength;
		BufferFormat = _format;
	}

	~TCompositeBuffer()
	{
		if (Data)
		{
			delete [] Data;
			Data = 0;
		}
	}

	inline int ItemCapacity()
	{
		return BufferFormat->ItemCapacity(Capacity);
	}

	/*void Nullify()
	{
		Data = 0;		// Buffer start pointer
		Index = 0;	// Current write pointer

		CapacityItem = 0;		// Buffer capacity in item count
		Capacity = 0;	// Buffer capacity in bytes
		Used = 0;			// Buffer usage in item count

		BufferFormat = 0;	// Format definition
	}

	void Use(byte* newBuffer, int newItemCap,int newUsed = 0);*/

	void ExchangeBuffer(TCompositeBuffer* excBuffer);

	void ConvertCopy(TCompositeBuffer* otherBuffer);

	/**
	* Creates a empty flexible buffer and copies self data, then converts it.
	* @return 0 if no converter available, if available returns newly created buffer.
	*/
	TCompositeBuffer* ConvertCopy(TBufferFormat* newFormat);

	bool Convert(TBufferFormat* newFormat);

	inline void AddFloat(float value)
	{
		*(float*)Data[Index] = value;
		Index += 4;
	}

	inline void AddShort(short value)
	{
		*(short*)Data[Index] = value;
		Index += sizeof(short);
	}

	inline void AddInt(int value)
	{
		*(int*)Data[Index] = value;
		Index += sizeof(int);
	}

	inline void AddDword(dword value)
	{
		*(dword*)Data[Index] = value;
		Index += 4;
	}

	inline void AddByte(byte value)
	{
		*(byte*)Data[Index] = value;
		Index++;
	}
};

#endif