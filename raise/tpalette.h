#ifndef TPALETTE_H
#define TPALETTE_H

#include "tarray.h"
#include "tcolor.h"

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
		Colors[index] = color;
	}
};

#endif