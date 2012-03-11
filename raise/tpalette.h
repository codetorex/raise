#ifndef TPALETTE_H
#define TPALETTE_H

#include "tarray.h"
#include "tcolor.h"
#include "texceptionlow.h"

class TPalette
{
public:

	TArray< TColor32 > Colors;

	int ColorCount;

	inline void Initialize(ui32 size)
	{
		Colors.Allocate(size);
		ColorCount = size;
	}

	inline TColor32& GetColor(ui32 index)
	{
		return Colors[index];
	}

	inline void SetColor(ui32 index, TColor32 color)
	{
		if (index < 0 || index >= Colors.Capacity)
		{
			LowLevelException("Palette index out of bounds?");
		}
		Colors[index] = color;
	}

	/*inline void AddColors(ui32* array, ui32 arraysize)
	{
		int totalCount = Colors.Count + arraysize;
		if (Colors.Capacity < totalCount)
		{
			Initialize(totalCount);
		}

		for (int i=0;i< arraysize;i++)
		{
			SetColor(i, array[i]);
		}
	}*/

	TPalette( TColor32 _array[], ui32 _arraysize ): Colors(_array,_arraysize)
	{
		
	}

	/*inline void AddColors(TArray< TColor32 >* colorArray)
	{

	}*/
};

#endif