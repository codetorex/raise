#ifndef TSTACK_H
#define TSTACK_H

#include "raisetypes.h"

template <class T>
class RDLL TStack
{
public:
	T		CurItem;
	T*		Item;
	dword	Count;
	dword	Capacity;

	TStack()
	{
		Item = 0;
		Allocate(8);
		Count = 0;
	}

	TStack(int initialCapacity)
	{
		Item = 0;
		Count = 0;
		Allocate(initialCapacity);
	}

	inline void Push(T value)
	{
		if (Count == Capacity)
		{
			Allocate(MUL2(Capacity));
		}
		Item[Count++] = value;
		CurItem = value;
	}

	inline T Pop()
	{
		T rt = Item[--Count];
		if (Count != 0)
		{
			CurItem = Item[Count-1];
		}
		else
		{
			CurItem = NULL;
		}
		return rt;
	}

	inline T Peek()
	{
		return CurItem;
	}


protected:
	void Allocate(int newsize)
	{
		T* NItem = new T [newsize];
		if (Item)
		{
			memcpy(NItem,Item,sizeof(T) * Count);
			delete [] Item;
		}
		Item = NItem;
		Capacity = newsize;
	}
};

#endif