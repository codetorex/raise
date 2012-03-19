#ifndef TQUEUE_H
#define TQUEUE_H

#include "raisetypes.h"


// TODO: test queue

template <class T>
class RDLL TQueue
{
public:
	T*		Item;
	ui32	Count;
	ui32	Capacity;

	ui32 Head;
	ui32 Tail;

	TQueue()
	{
		Item = 0;
		Allocate(8);
		Count = 0;
		Head = 0;
		Tail = 0;
	}

	TQueue(int initialCapacity)
	{
		Item = 0;
		Count = 0;
		Head = 0;
		Tail = 0;
		Allocate(initialCapacity);
	}

	~TQueue()
	{
		delete [] Item;
		Item = 0;
		Count = 0;
		Capacity = 0;
		Head = 0;
		Tail = 0;
	}

	inline void Push(T value)
	{
		if (Count == Capacity)
		{
			Allocate(MUL2(Capacity));
		}
		Item[Tail] = value;
		Count++;

		Tail++;
		if (Tail >= Capacity)
		{
			Tail = 0;
		}
	}

	inline T Pop()
	{
		T result = Item[Head];
		Count--;

		Head++;
		if (Head >= Capacity)
		{
			Head = 0;
		}

		return result;
	}

	inline T Peek()
	{
		return Item[Head];
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