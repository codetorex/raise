
#ifndef TARRAY_H
#define TARRAY_H

#include "raisetypes.h"

#include <vector>

/**
* Order not preserved in this implementation.
*/

template <class T>
class TArray
{
public:
	TArray()
	{
		Allocate(8);
		Count = 0;
	}

	TArray(int size)
	{
		Allocate(size);
		Count = 0;
	}

	T*		Item;
	dword	Count;
	dword	Capacity;

	void	Insert(T value,int index);
	
	void	Remove(T value)
	{
		
	}

	void	RemoveAt(int index)
	{
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

	int		IndexOf(T value);
	int		LastIndexOf(T value);
	bool	Contains(T value);

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