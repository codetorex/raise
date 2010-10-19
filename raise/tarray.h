
#ifndef TARRAY_H
#define TARRAY_H

#include "raisetypes.h"

#define TARRAYDEFSIZE	4


template<class T,int Sz>
class TStaticArray
{
public:
	T	Item[Sz];
	int Count;

	TStaticArray()
	{
		Count = 0;
	}

	inline int LastIndexOf(T value)
	{
		int c = Sz;
		while(c--)
		{
			if (Item[c] == value)
			{
				return c;
			}
		}
		return -1;
	}

	inline void RemoveAt(int index)
	{
		if (index == (Sz -1))
		{
			Count--;
			return;
		}
		Item[index] = Item[--Count];
	}

	inline void Remove(T value)
	{
		int i = LastIndexOf(value);
		if (i != -1)
		{
			RemoveAt(i);
		}
	}

	inline void	Add(T value)
	{
		Item[Count++] = value;
	}
};


/**
* Order not preserved in this implementation.
*/

template <class T>
class TArray
{
public:
	TArray()
	{
		Item = 0;
		Allocate(TARRAYDEFSIZE);
		Count = 0;
	}

	TArray(int size)
	{
		Item = 0;
		Allocate(size);
		Count = 0;
	}

	T*		Item;
	dword	Count;
	dword	Capacity;

	void	Insert(T value,int index);
	

	inline int IndexOf(T value)
	{
		for (int i=0;i<Count;i++)
		{
			if (Item[i] == value)
			{
				return i;
			}
		}
		return -1;
	}

	inline int LastIndexOf(T value)
	{
		int c = Count;
		while(c--)
		{
			if (Item[c] == value)
			{
				return c;
			}
		}
		return -1;
	}

	inline void Remove(T value)
	{
		int i = LastIndexOf(value);
		if (i != -1)
		{
			RemoveAt(i);
		}
	}

	inline void RemoveAt(int index)
	{
		if (index == Count -1)
		{
			Count--;
			return;
		}
		Item[index] = Item[--Count];
	}

	void	Clear();
	
	inline void	Add(T value)
	{
		if (Capacity <= Count)
		{
			Allocate(Capacity<<1); // Multiply the cache
		}
		Item[Count++] = value;
	}

	/**
	* If you know data will not get out of bounds, use this.
	*/
	inline void UnsafeAdd(T value)
	{
		Item[Count++] = value;
	}




	bool Contains(T value)
	{
		int i = LastIndexOf(value);
		if (i != -1)
		{
			return true;
		}
		return false;
	}

	void	Reverse();
	void	Sort();

	inline T& operator [] (int index)
	{
		return Item[index];
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