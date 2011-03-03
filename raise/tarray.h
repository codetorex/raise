
#ifndef TARRAY_H
#define TARRAY_H

#include "raisetypes.h"
#include "tmemorydriver.h"

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
		Capacity = 0;
		Count = 0;
	}

	TArray(int size)
	{
		Item = 0;
		Allocate(size);
		Count = 0;
	}

	TArray( const TArray<T>& cpy)
	{
		Item = 0;
		Count = 0;
		Allocate(cpy.Capacity);
		MemoryDriver::Copy(Item,cpy.Item,sizeof(T) * cpy.Count);
		Count = cpy.Count;
	}

	~TArray()
	{
		Free();
		Item = 0;
		Count = 0;
		Capacity = 0;
	}

	T*		Item;
	dword	Count;
	dword	Capacity;

	void	Insert(T value,int index);
	
	/**
	* When no changes needed anymore, you can use this to create static array with length of count. (without additional capacity)
	*/
	inline T* ExtractItems()
	{
		T* nItems = new T [Count];
		memcpy(nItems,Item,sizeof(T) * Count);
		return nItems;
	}

	/**
	* Generic counter for zero ending array.
	*/
	static inline int CountOfZeroEnding(T* _array)
	{
		for (int i = 0; true ; i++)
		{
			if (_array[i] == NULL)
			{
				return i;
				break;
			}
			if (i == 1024)
			{
				throw Exception("Impossible"); // TODO: if it will be possible someday change this.
			}
		}
	}

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

	inline void RemoveLast()
	{
		Count--;
		return;
	}

	void Clear()
	{
		memset(Item,NULL, sizeof(T) * Count);
		Count = 0;
	}
	
	inline void	Add(T value)
	{
		if (Capacity <= Count)
		{
			if (Capacity == 0) Capacity = 2;
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

	inline void Free()
	{
		if (Item)
		{
			delete [] Item;
		}
	}

	inline T operator [] (int index)
	{
		return Item[index];
	}

	inline T GetLast()
	{
		if (Count > 0)
		{
			return Item[Count-1];
		}
		return NULL;
	}

	/**
	* Assigning of arrays will destroy other.
	*/
	inline T& operator = (TArray<T>& other)
	{
		Free();
		Item = other.Item;
		Count = other.Count;
		Capacity = other.Capacity;
		other.Item = 0;
		other.Count = 0;
		other.Capacity = 0;
		return *this;
	}



protected:
	void Allocate(int newsize)
	{
		if (newsize == 0)
		{
			Capacity = 0;
			Item = 0;
			Count = 0;
			throw Exception("Tried to allocate 0, uncessary");
			return;
		}

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