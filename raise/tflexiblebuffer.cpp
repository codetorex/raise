#include "stdafx.h"
#include "tflexiblebuffer.h"
#include "texception.h"


void TFormatElementGroup::CreateElementList( TArray<TFormatElement*>* allElements, const str8& elementNames )
{
	ElementCount = 0;

	int curPos = 0;

	TArray<TFormatElement*> newList;
	while (curPos < elementNames.Length)
	{
		bool found = false;
		for (dword i=0;i<allElements->Count;i++)
		{
			TFormatElement* curElement = allElements->Item[i];
			if (elementNames.StartsWith(curElement->ShortName,curPos))
			{
				newList.Add(curElement);
				curPos += curElement->ShortName.Length;
				ElementCount++;
				found = true;
				break;
			}
		}

		if (!found)
		{
			throw Exception("Missing channel");
		}
	}

	UseElementList(newList.ExtractItems(),ElementCount);
}

int TFormatElementGroup::CalculateBitsPerItem()
{
	int result = 0;
	int i = ElementCount;
	while(i--)
	{
		switch(Elements[i]->DataType)
		{
		case tc_bit1:
			result += 1;
			break;

		case tc_bit2:
			result += 2;
			break;

		case tc_bit3:
			result += 3;
			break;

		case tc_bit4:
			result += 4;
			break;

		case tc_bit5:
			result += 5;
			break;

		case tc_bit6:
			result += 6;
			break;

		case tc_bit7:
			result += 7;
			break;

		case tc_bit8:
			result += 8;
			break;

		case tc_byte:
			result += 8;
			break;

		case tc_short:
			result += 16;
			break;

		case tc_dword:
		case tc_float:
			result += 32;
			break;

		case tc_double:
			result += 64;
			break;

		case tc_float24:
			result += 24;
			break;

		case tc_group:
			TFormatElementGroup* elemGroup = (TFormatElementGroup*)Elements[i];
			result += elemGroup->BitsPerItem;
			break;
		}
	}
	return result;
}

TBufferFormatConverter* TFormatElementGroup::GetConverter( TBufferFormat* TargetFormat )
{
	for(dword i=0;i<Converters.Count;i++)
	{
		if (Converters.Item[i]->DestinationFormat == TargetFormat)
		{
			return Converters.Item[i];
		}
	}

	return 0;
}
void TFlexibleBuffer::Allocate( int _newItemCapacity )
{
	if (_newItemCapacity < Capacity)
	{
		Indicator = Buffer;
		Used = 0;
		return;
	}

	if (Buffer)
	{
		Free();
	}

	if (_newItemCapacity == 0)
	{
		return;
	}

	Capacity = _newItemCapacity;
	CapacityByte = _newItemCapacity * BufferFormat->BytesPerItem;
	Buffer = new byte [CapacityByte];
	Used = 0;
}

void TFlexibleBuffer::Grow( int _newItemCapacity )
{
	if (Capacity > _newItemCapacity)
	{
		Indicator = Buffer;
		Used = 0;
		return;
	}
	byte* OldBuffer = Buffer;
	CapacityByte = _newItemCapacity * BufferFormat->BytesPerItem;
	Buffer = new byte [CapacityByte];

	MemoryDriver::Copy(Buffer,OldBuffer,Used * BufferFormat->BytesPerItem);
	Indicator = Buffer + (Indicator - OldBuffer);
	Capacity = _newItemCapacity;
	delete [] OldBuffer;
}

void TFlexibleBuffer::Initialize( TBufferFormat* _format /*= 0*/,int _itemCapacity /*= 0*/ )
{
	Capacity = _itemCapacity;
	Used = 0;
	BufferFormat = _format;
	Buffer = 0;
	if (Capacity != 0)
	{
		Allocate(Capacity);
	}
}