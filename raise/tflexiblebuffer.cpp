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
		for (int i=0;i<allElements->Count;i++)
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

void TFlexibleBuffer::Allocate( int _newCapacity )
{
	if (_newCapacity < Capacity)
	{
		Indicator = Buffer;
		Used = 0;
		return;
	}

	if (Buffer)
	{
		Free();
	}

	if (_newCapacity == 0)
	{
		return;
	}

	Capacity = _newCapacity;
	Buffer = new byte [_newCapacity];
	Used = 0;
}

void TFlexibleBuffer::Grow( int _newCapacity )
{
	if (Capacity > _newCapacity)
	{
		Indicator = Buffer;
		Used = 0;
		return;
	}
	byte* OldBuffer = Buffer;
	Buffer = new byte [_newCapacity];
	memcpy(Buffer,OldBuffer,Used);
	Indicator = Buffer + (Indicator - OldBuffer);
	Capacity = _newCapacity;
	delete [] OldBuffer;
}

void TFlexibleBuffer::Initialize( TBufferFormat* _format /*= 0*/,int _capacity /*= 0*/ )
{
	Capacity = _capacity;
	Used = 0;
	BufferFormat = _format;
	Buffer = 0;
	if (Capacity != 0)
	{
		Allocate(Capacity);
	}
}