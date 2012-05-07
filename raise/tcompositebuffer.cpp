#include "stdafx.h"
#include "tcompositebuffer.h"
#include "tcompositeconverter.h"

void TCompositeBuffer::AllocateItemCapacity( int _newItemCapacity )
{
	if (_newItemCapacity < CapacityItem)
	{
		Index = 0;
		Used = 0;
		return;
	}

	if (Data)
	{
		Free();
	}

	if (_newItemCapacity == 0)
	{
		return;
	}

	CapacityItem = _newItemCapacity;
	Capacity = _newItemCapacity * BufferFormat->BytesPerItem;
	Data = new byte [Capacity];
	Used = 0;
}

void TCompositeBuffer::GrowItemCapacity( int _newItemCapacity )
{
	if (CapacityItem > _newItemCapacity)
	{
		Index = 0;
		Used = 0;
		return;
	}
	byte* OldBuffer = Data;
	Capacity = _newItemCapacity * BufferFormat->BytesPerItem;
	Data = new byte [Capacity];

	MemoryDriver::Copy(Data,OldBuffer,Used * BufferFormat->BytesPerItem);
	CapacityItem = _newItemCapacity;
	delete [] OldBuffer;
}

void TCompositeBuffer::AllocateItemCapacity( TBufferFormat* _format /*= 0*/,int _itemCapacity /*= 0*/ )
{
	CapacityItem = _itemCapacity;
	Used = 0;
	BufferFormat = _format;
	Data = 0;
	if (CapacityItem != 0)
	{
		AllocateItemCapacity(CapacityItem);
	}
}

void TCompositeBuffer::ExchangeBuffer( TCompositeBuffer* excBuffer )
{
	byte tmpMemory[512];
	MemoryDriver::Copy(tmpMemory,excBuffer,sizeof(TCompositeBuffer));
	MemoryDriver::Copy(excBuffer,this,sizeof(TCompositeBuffer));
	MemoryDriver::Copy(this,tmpMemory,sizeof(TCompositeBuffer));

	/*TFlexibleBuffer tmp = *excBuffer;
	*excBuffer = *this;
	*this = tmp;*/
}

void TCompositeBuffer::ConvertCopy( TCompositeBuffer* otherBuffer )
{
	TCompositeConverter* Converter = BufferFormat->GetConverter(otherBuffer->BufferFormat);
	if (Converter == 0)
	{
		throw Exception("No converter");
	}

	Converter->Convert(this,otherBuffer);
}

TCompositeBuffer* TCompositeBuffer::ConvertCopy( TBufferFormat* newFormat )
{
	if (BufferFormat == newFormat)
	{
		throw Exception("No conversion needed. Is this bug?");
	}

	TCompositeConverter* Converter = BufferFormat->GetConverter(newFormat);
	if (Converter == 0)
	{
		return 0;
	}

	TCompositeBuffer* newBuffer = newFormat->CreateBuffer(CapacityItem);
	Converter->Convert(this,newBuffer);
	return newBuffer;
}

bool TCompositeBuffer::Convert( TBufferFormat* newFormat )
{
	if (BufferFormat == newFormat)
	{
		return true;
	}

	TCompositeConverter* Converter = BufferFormat->GetConverter(newFormat);

	if (Converter == 0)
	{
		throw Exception("Converter not found");
	}

	Converter->Convert(this);
	return true;
	// TODO: IMPLEMENT A GENERIC FORMAT EXCHANGING VIRTUAL MACHINE AND X86 COMPILER HERE
}

/*void TCompositeBuffer::Use( byte* newBuffer, int newItemCap,int newUsed )
{
	if (Data)
	{
		delete Data;
		Data = 0;
	}

	Data = newBuffer;
	Index = 0;
	CapacityItem = newItemCap;
	Used = newUsed;
}*/