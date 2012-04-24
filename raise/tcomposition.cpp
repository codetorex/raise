#include "stdafx.h"
#include "tcompositebuffer.h"
#include "texception.h"
#include "tcompositionmanager.h"

char* PrimitiveNames[] = 
{
		"tc_byte",
		"tc_short",
		"tc_dword",
		"tc_float",
		"tc_double",
		"tc_bit1",
		"tc_bit2",
		"tc_bit3",
		"tc_bit4",
		"tc_bit5",
		"tc_bit6",
		"tc_bit7",
		"tc_bit8",
		"tc_group",
		"tc_float24",
		"tc_notdefined",
};

TCompositeFormatFactoryGeneric TCompositeFormatFactoryGeneric::Instance;

void TComposition::CreateElementList( TArray<TCompositionPrimitive*>* allElements, const TString& elementNames )
{
	ui32 curPos = 0;

	TArray<TCompositionPrimitive*>* newList = new TArray<TCompositionPrimitive*>();
	while (curPos < elementNames.Length)
	{
		bool found = false;
		for (ui32 i=0;i<allElements->Count;i++)
		{
			TCompositionPrimitive* curElement = allElements->Item[i];
			if (elementNames.StartsWith(curElement->ShortName,curPos))
			{
				newList->Add(curElement);
				curPos += curElement->ShortName.Length;
				found = true;
				break;
			}
		}

		if (!found)
		{
			throw Exception("Missing channel");
		}
	}

	UseElementList(newList);
}

int TComposition::CalculateBitsPerItem()
{
	int result = 0;

	TArrayEnumerator< TCompositionPrimitive* > ae(*Elements);
	while(ae.MoveNext())
	{
		switch(ae.Current->DataType)
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
			TComposition* elemGroup = (TComposition*)ae.Current;
			result += elemGroup->BitsPerItem;
			break;
		}
	}
	return result;
}

TCompositeConverter* TComposition::GetConverter( TBufferFormat* TargetFormat )
{
	for(ui32 i=0;i<Converters.Count;i++)
	{
		if (Converters.Item[i]->DestinationFormat == TargetFormat)
		{
			return Converters.Item[i];
		}
	}

	return 0;
}

TCompositeBuffer* TComposition::CreateBuffer( int _itemCapacity )
{
	return new TCompositeBuffer(this, _itemCapacity );
}