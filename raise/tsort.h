#ifndef TSORT_H
#define TSORT_H

#include "tmemorydriver.h"

/**
 * There could be better (generic) ways to accomplish these stuff.
 * But currently I just require these.
 */

class TSortInterface
{
public:
	virtual void SortObjects(void* srcArray, ui32 itemSize, ui32* currentOrder, ui32 itemCount) = 0;
};

class TSortBubble: public TSortInterface
{
public:

	static TSortBubble Instance;

	void SortObjects(void* srcArray, ui32 itemSize, ui32* currentOrder, ui32 itemCount);
};

#endif