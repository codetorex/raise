#include "stdafx.h"
#include "tcompositebuffer.h"
#include "texception.h"

void TComposition::CreateElementList( TArray<TCompositionPrimitive*>* allElements, const TString& elementNames )
{
	ElementCount = 0;

	dword curPos = 0;

	TArray<TCompositionPrimitive*> newList;
	while (curPos < elementNames.Length)
	{
		bool found = false;
		for (dword i=0;i<allElements->Count;i++)
		{
			TCompositionPrimitive* curElement = allElements->Item[i];
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

int TComposition::CalculateBitsPerItem()
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
			TComposition* elemGroup = (TComposition*)Elements[i];
			result += elemGroup->BitsPerItem;
			break;
		}
	}
	return result;
}

TCompositeConverter* TComposition::GetConverter( TBufferFormat* TargetFormat )
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

TCompositeBuffer* TComposition::CreateBuffer( int _itemCapacity )
{
	return new TCompositeBuffer(this, _itemCapacity );
}