#ifndef TQUEUE_H
#define TQUEUE_H

#include "raisetypes.h"
#include "texceptionlow.h"


// TODO: test queue

template <class T>
class RDLL TQueue
{
public:
	T*		Item;
	ui32	Count;
	ui32	Capacity;

	ui32	Head;
	ui32	Tail;

	TQueue()
	{
		Item = 0;
		Clear();
		Allocate(8);
	}

	TQueue(int initialCapacity)
	{
		Item = 0;
		Clear();
		Allocate(initialCapacity);
	}

	~TQueue()
	{
		delete [] Item;
		Item = 0;
		Capacity = 0;
		Clear();
	}

	/**
	 * Enqueue array of items.
	 */
	void Enqueue( T* Source, ui32 Length )
	{
		ui32 CapRequired = Count + Length;
		if ( CapRequired > Capacity)
		{
			DoubleUntil(CapRequired);
		}

		if (Head < Tail) // its continuous lets write data
		{
			ui32 continousLength = Capacity - Tail;
			if (continousLength >= Length) // Great
			{
				MemoryDriver::Copy(Item + Tail,Source,Length * sizeof(T));
			}
			else // continuous part is not enough to full fill, splitting data
			{
				MemoryDriver::Copy(Item + Tail, Source, continousLength * sizeof(T));
				MemoryDriver::Copy(Item, Source + continousLength, (Length - continousLength) * sizeof(T));
			}
		}
		else
		{
			MemoryDriver::Copy(Item + Tail, Source,Length * sizeof(T));
		}

		Tail = (Tail + Length) % Capacity;
		Count += Length;
	}

	/**
	 * Dequeue array of items.
	 * @return ui32 Amount of data dequeued.
	 */
	ui32 Dequeue( T* Destination, ui32 Length)
	{
		if (Length > Count)
		{
			Length = Count;
		}

		if (Length == 0)
		{
			return 0;
		}

		if (Head < Tail) // its continuous data we can read easily
		{
			MemoryDriver::Copy(Destination, Item + Head, Length * sizeof(T));
		}
		else
		{
			ui32 continousLength = Capacity - Head;
			if (continousLength > Length)
			{
				MemoryDriver::Copy(Destination, Item + Head, Length * sizeof(T));
			}
			else
			{
				MemoryDriver::Copy(Destination, Item + Head, continousLength * sizeof(T));
				MemoryDriver::Copy(Destination + continousLength, Item, (Length - continousLength) * sizeof(T));
			}
		}
		
		Head = (Head + Length) % Capacity;
		Count -= Length;

		if ( Count == 0)
		{
			Head = 0;
			Tail = 0;
		}

		return Length;
	}

	inline void Enqueue(T value)
	{
		if (Count >= Capacity)
		{
			DoubleCapacity();
		}
		Item[Tail] = value;
		Count++;

		Tail++;
		if (Tail >= Capacity)
		{
			Tail = 0;
		}
	}

	inline T Dequeue()
	{
		if (Count)
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

		LowLevelException("Invalid Operation: No data present in queue");
		return 0;
	}

	inline T Peek()
	{
		return Item[Head];
	}

	inline void Clear()
	{
		Head = 0;
		Tail = 0;
		Count = 0;
	}


protected:
	inline void DoubleCapacity()
	{
		Allocate(Capacity * 2);
	}

	inline void DoubleUntil(ui32 newCap)
	{
		ui32 curcap = Capacity;
		while(curcap < newCap)
		{
			curcap *= 2;
		}
		Allocate(curcap);
	}

	void Allocate(ui32 newsize)
	{
		if (newsize < Capacity)
		{
			if (Count > newsize)
			{
				LowLevelException( "Tried to allocate lesser capacity than existing array" );
			}
		}

		T* NItem = new T [newsize];
		if (Item)
		{
			if (Tail < Head) // if parts are divided
			{
				ui32 rem = Capacity - Head; // data on right part
				MemoryDriver::Copy(NItem, Item + Head,sizeof(T) * rem); // copy data in right part to beginning of new array
				MemoryDriver::Copy(NItem + rem, Item, Tail); // copy data on left part after right part
				// Result will be unified continuous array
			}
			else
			{
				MemoryDriver::Copy(NItem,Item + Head,sizeof(T) * Count);
			}

			Head = 0;
			Tail = Count;
			delete [] Item;
		}
		Item = NItem;
		Capacity = newsize;
	}
};

typedef TQueue<byte> TByteQueue;

#endif