#include "stdafx.h"
#include "tsort.h"


TSortBubble TSortBubble::Instance;

void TSortBubble::SortObjects( void* srcArray, ui32 itemSize, ui32* currentOrder, ui32 itemCount )
{
	for(int i=0;i<itemCount;i++)
	{
		for(int j=0;j<i;j++)
		{
			if(currentOrder[i]<currentOrder[j])
			{
				int temp=currentOrder[i]; //swap 
				currentOrder[i]=currentOrder[j];
				currentOrder[j]=temp;

				byte* src = (byte*)srcArray;
				src += itemSize * i;

				byte* dst = (byte*)srcArray;
				dst += itemSize * j;

				MemoryDriver::Exchange(src,dst,itemSize);
			}
		}
	}
}