#ifndef TCOMPOSITION_H
#define TCOMPOSITION_H


#include "tcompositionprimitive.h"

class TCompositeBuffer;
class TCompositeConverter;
class TComposition;
typedef TComposition TBufferFormat;

typedef TArray<TCompositionPrimitive*> PrimitiveArray;

/**
* Holds multiple element information and unites as a group or format definition.
*/
class TComposition: public TCompositionPrimitive
{
public:
	TArray<TCompositionPrimitive*>* Elements;
	TArray<TCompositeConverter*> Converters;
	int BytesPerItem;
	int BitsPerItem;

	void UseElementList(TArray<TCompositionPrimitive*>* _elements)
	{
		Elements = _elements;
		BitsPerItem = CalculateBitsPerItem();
		BytesPerItem = DIV8(BitsPerItem);
	}

	TComposition()
	{
		Elements = 0;
		BytesPerItem = 0;
		BitsPerItem = 0;
		DataType = tc_group;
	}

	TComposition(ui32 _bytePerItem)
	{
		Name = "UNKNOWN";
		ShortName = "UNKNOWN";
		DataType = tc_group;
		BytesPerItem = _bytePerItem;
		Elements = 0;
		BitsPerItem = BytesPerItem * 8;
	}

	TComposition(const TString& _Name, const TString& _short,TArray<TCompositionPrimitive*>* _elements): TCompositionPrimitive(_Name,_short, tc_group)
	{
		UseElementList(_elements);
	}

	void CreateElementList(TArray<TCompositionPrimitive*>* allElements, const TString& elementNames);

	/**
	* With this constructor, just give pointer to all elements with zero pointer ending, then use a string that defines this format by short names of elements
	* Like: TBufferFormat( "BGRA" , ptrToAllElements, 4 , "B8G8R8A8" )
	* It automatically adds necessary elements by ordering that given.
	*/
	TComposition(const TString& _Name, const TString& _short,PrimitiveArray* _elements, const TString& elementNames): TCompositionPrimitive(_Name,_short,tc_group)
	{
		CreateElementList(_elements,elementNames);
	}

	TComposition(const TString& _Name,PrimitiveArray* _elements, const TString& elementNames): TCompositionPrimitive(_Name,_Name,tc_group)
	{
		CreateElementList(_elements,elementNames);
	}

	
	TCompositeConverter* GetConverter(TBufferFormat* TargetFormat);

	inline bool HaveConverter(TBufferFormat* TargetFormat)
	{
		TCompositeConverter* conv = GetConverter(TargetFormat);
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

	TCompositeBuffer* CreateBuffer(int _itemCapacity);

	TString ToString();
};

#endif