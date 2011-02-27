#ifndef TFLEXIBLEBUFFER_H
#define TFLEXIBLEBUFFER_H

#include "raisetypes.h"
#include "tstring.h"

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

	int CalculateBitsPerItem();
};

typedef TFormatElementGroup TBufferFormat;

class TFlexibleBuffer;

/**
* Will be interface of converter providers.
*/
class TBufferFormatConverter
{
public:
	void Convert(TFlexibleBuffer* src, TBufferFormat* dst) = 0;
};

/**
* Buffer that is flexible in format definition.
*/
class TFlexibleBuffer
{
public:
	byte* Buffer;		// Buffer start pointer
	byte* Indicator;	// Current write pointer

	int Capacity;		// Buffer capacity in bytes
	int Used;			// Buffer usage in bytes

	TBufferFormat* BufferFormat;	// Format definition

	void Free()
	{
		if (Buffer)
		{
			delete [] Buffer;
		}
		Buffer = Indicator = 0;
		Capacity = Used = 0;
	}

	void Allocate(int _newCapacity);

	void Grow(int _newCapacity);

	void Initialize(TBufferFormat* _format = 0,int _capacity = 0);


	TFlexibleBuffer()
	{
		Initialize();
	}

	TFlexibleBuffer(TBufferFormat* _format,int _itemCapacity)
	{
		Initialize(_format,_itemCapacity * _format->BytesPerItem);
	}

	TFlexibleBuffer(TBufferFormat* _format,byte* _buffer,int _bufferLength)
	{
		Buffer = Buffer;
		Indicator = Buffer + _bufferLength;
		Used = Capacity = _bufferLength;
		BufferFormat = _format;
	}

	void Convert(TBufferFormat* newFormat)
	{

		// IMPLEMENT A GENERIC FORMAT EXCHANGING VIRTUAL MACHINE AND X86 COMPILER HERE
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



#endif