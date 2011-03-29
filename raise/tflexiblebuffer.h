#ifndef TFLEXIBLEBUFFER_H
#define TFLEXIBLEBUFFER_H

#include "raisetypes.h"
#include "texception.h" // includes string too

enum TComponentTypes
{
	tc_byte,
	tc_short,
	tc_dword,
	tc_float,
	tc_double,
	tc_bit1,
	tc_bit2,
	tc_bit3,
	tc_bit4,
	tc_bit5,
	tc_bit6,
	tc_bit7,
	tc_bit8,
	tc_group,
	tc_float24,
	tc_notdefined,
};

/**
* Single element of a format definition.
*/
class TFormatElement
{
public:
	str8 Name;
	str8 ShortName;
	int DataType;

	TFormatElement()
	{
		DataType = tc_notdefined;
	}

	TFormatElement(const str8& _name, const str8& _short, int _dataType)
	{
		Name = _name;
		ShortName = _short;
		DataType = _dataType;
	}
};

class TBufferFormatConverter;
class TFormatElementGroup;
class TFlexibleBuffer;

typedef TFormatElementGroup TBufferFormat;

/**
* Will be interface of converter providers.
*/
class TBufferFormatConverter
{
public:
	TBufferFormat* SourceFormat; // TODO: use reference here?
	TBufferFormat* DestinationFormat;

	/**
	* In-place converting virtual function.
	*/
	virtual void Convert(TFlexibleBuffer* srcBuffer) = 0;

	/**
	* Uses new buffer for conversion result.
	*/
	virtual void Convert(TFlexibleBuffer* srcBuffer,TFlexibleBuffer* dstBuffer) = 0;
};


/**
* Holds multiple element information and unites as a group or format definition.
*/
class TFormatElementGroup: public TFormatElement
{
public:
	TFormatElement** Elements;
	int ElementCount;
	int BytesPerItem;
	int BitsPerItem;

	TArray<TBufferFormatConverter*> Converters;

	void UseElementList(TFormatElement** _elements, int count)
	{
		Elements = _elements;
		ElementCount = count;
		BitsPerItem = CalculateBitsPerItem();
		BytesPerItem = DIV8(BitsPerItem);
	}

	TFormatElementGroup()
	{
		Elements = 0;
		ElementCount = 0;
		BytesPerItem = 0;
		BitsPerItem = 0;
	}

	TFormatElementGroup(const str8& _Name, const str8& _short,TFormatElement** _elements,int count): TFormatElement(_Name,_short, tc_group)
	{
		UseElementList(_elements,count);
	}

	void CreateElementList(TArray<TFormatElement*>* allElements, const str8& elementNames);

	/**
	* With this constructor, just give pointer to all elements with zero pointer ending, then use a string that defines this format by short names of elements
	* Like: TBufferFormat( "BGRA" , ptrToAllElements, 4 , "B8G8R8A8" )
	* It automatically adds necessary elements by ordering that given.
	*/
	TFormatElementGroup(const str8& _Name, const str8& _short,TArray<TFormatElement*>* _elements, const str8& elementNames): TFormatElement(_Name,_short,tc_group)
	{
		CreateElementList(_elements,elementNames);
	}

	TFormatElementGroup(const str8& _Name,TArray<TFormatElement*>* _elements, const str8& elementNames): TFormatElement(_Name,_Name,tc_group)
	{
		CreateElementList(_elements,elementNames);
	}

	
	TBufferFormatConverter* GetConverter(TBufferFormat* TargetFormat);

	inline bool HaveConverter(TBufferFormat* TargetFormat)
	{
		TBufferFormatConverter* conv = GetConverter(TargetFormat);
		if (conv == 0)
		{
			return false;
		}
		return true;
	}



	int CalculateBitsPerItem();

	inline int ByteCapacity(int ItemCount)
	{
		return ItemCount * BytesPerItem;
	}

	inline int ItemCapacity(int Capacity)
	{
		return Capacity / BytesPerItem;
	}

	inline byte* AllocateBuffer(int _itemcapacity)
	{
		return new byte [ ByteCapacity(_itemcapacity) ];
	}

	inline TFlexibleBuffer* CreateBuffer(int _itemCapacity);
};


/**
* Buffer that is flexible in format definition.
*/
class TFlexibleBuffer
{
public:
	byte* Buffer;		// Buffer start pointer
	byte* Indicator;	// Current write pointer

	int Capacity;		// Buffer capacity in item count
	int CapacityByte;	// Buffer capacity in bytes
	int Used;			// Buffer usage in item count

	TBufferFormat* BufferFormat;	// Format definition

	void Free()
	{
		if (Buffer)
		{
			delete [] Buffer;
			Buffer = 0;
		}
		Indicator = 0;
		Capacity = Used = 0;
	}

	void Allocate(int _newItemCapacity);

	void Grow(int _newItemCapacity);

	void Initialize(TBufferFormat* _format = 0,int _itemCapacity = 0);

	inline byte* GetBufferEnd()
	{
		return (Buffer + CapacityByte);
	}

	TFlexibleBuffer()
	{
		Initialize();
	}

	TFlexibleBuffer(TBufferFormat* _format,int _itemCapacity)
	{
		Initialize(_format,_itemCapacity);
	}

	TFlexibleBuffer(TBufferFormat* _format,byte* _buffer,int _bufferLength)
	{
		Buffer = Buffer;
		Indicator = Buffer + _bufferLength;
		Used = Capacity = _bufferLength;
		BufferFormat = _format;
	}

	~TFlexibleBuffer()
	{
		if (Buffer)
		{
			delete [] Buffer;
			Buffer = 0;
		}
	}

	inline int ItemCapacity()
	{
		return BufferFormat->ItemCapacity(Capacity);
	}

	void Nullify()
	{
		Buffer = 0;		// Buffer start pointer
		Indicator = 0;	// Current write pointer

		Capacity = 0;		// Buffer capacity in item count
		CapacityByte = 0;	// Buffer capacity in bytes
		Used = 0;			// Buffer usage in item count

		BufferFormat = 0;	// Format definition
	}

	void UseBuffer(byte* newBuffer, int newItemCap,int newUsed = 0)
	{
		if (Buffer)
		{
			delete Buffer;
			Buffer = 0;
		}

		Buffer = newBuffer;
		Indicator = Buffer;
		Capacity = newItemCap;
		Used = newUsed;
	}

	void ExchangeBuffer(TFlexibleBuffer* excBuffer)
	{
		byte tmpMemory[512];
		MemoryDriver::Copy(tmpMemory,excBuffer,sizeof(TFlexibleBuffer));
		MemoryDriver::Copy(excBuffer,this,sizeof(TFlexibleBuffer));
		MemoryDriver::Copy(this,tmpMemory,sizeof(TFlexibleBuffer));

		/*TFlexibleBuffer tmp = *excBuffer;
		*excBuffer = *this;
		*this = tmp;*/
	}

	void ConvertCopy(TFlexibleBuffer* otherBuffer)
	{
		TBufferFormatConverter* Converter = BufferFormat->GetConverter(otherBuffer->BufferFormat);
		if (Converter == 0)
		{
			throw Exception("No converter");
		}

		Converter->Convert(this,otherBuffer);
	}

	/**
	* Creates a empty flexible buffer and copies self data, then converts it.
	* @return 0 if no converter available, if available returns newly created buffer.
	*/
	TFlexibleBuffer* ConvertCopy(TBufferFormat* newFormat)
	{
		if (BufferFormat == newFormat)
		{
			throw Exception("No conversion needed. Is this bug?");
		}

		TBufferFormatConverter* Converter = BufferFormat->GetConverter(newFormat);
		if (Converter == 0)
		{
			return 0;
		}

		TFlexibleBuffer* newBuffer = newFormat->CreateBuffer(Capacity);
		Converter->Convert(this,newBuffer);
		return newBuffer;
	}

	bool Convert(TBufferFormat* newFormat)
	{
		if (BufferFormat == newFormat)
		{
			return true;
		}

		TBufferFormatConverter* Converter = BufferFormat->GetConverter(newFormat);

		if (Converter == 0)
		{
			throw Exception("Converter not found");
		}

		Converter->Convert(this);
		return true;
		// TODO: IMPLEMENT A GENERIC FORMAT EXCHANGING VIRTUAL MACHINE AND X86 COMPILER HERE
	}

	inline void AddFloat(float value)
	{
		*(float*)Indicator = value;
		Indicator += 4;
	}

	inline void AddShort(short value)
	{
		*(short*)Indicator = value;
		Indicator += sizeof(short);
	}

	inline void AddInt(int value)
	{
		*(int*)Indicator = value;
		Indicator += sizeof(int);
	}

	inline void AddDword(dword value)
	{
		*(dword*)Indicator = value;
		Indicator += 4;
	}

	inline void AddByte(byte value)
	{
		*Indicator = value;
		Indicator++;
	}
};

class TBufferFormatGenericConverter: public TBufferFormatConverter
{
public:

	virtual void DoConversion(byte* src, byte* dst,int itemCount) = 0;

	void Convert(TFlexibleBuffer* srcBuffer)
	{
		int itemCount = srcBuffer->Capacity;
		TFlexibleBuffer* tmpBuffer = DestinationFormat->CreateBuffer(itemCount);

		DoConversion(srcBuffer->Buffer,tmpBuffer->Buffer,itemCount);

		srcBuffer->ExchangeBuffer(tmpBuffer);
		delete tmpBuffer;
	}

	void Convert(TFlexibleBuffer* srcBuffer,TFlexibleBuffer* dstBuffer)
	{
		int itemCount = srcBuffer->Capacity;
		dstBuffer->Initialize(DestinationFormat,itemCount);

		DoConversion(srcBuffer->Buffer,dstBuffer->Buffer,itemCount);
	}
};


inline TFlexibleBuffer* TFormatElementGroup::CreateBuffer(int _itemCapacity)
{
	return new TFlexibleBuffer(this, _itemCapacity );
}


#endif