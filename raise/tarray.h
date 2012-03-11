
#ifndef TARRAY_H
#define TARRAY_H

#include "raisetypes.h"
#include "tmemorydriver.h"
#include "texceptionlow.h"
#include "tenumerator.h"

#define TARRAYDEFSIZE	4

/**
* Order not preserved in this implementation.
*/
template <class T>
class TArray
{
public:
	T*		Item;
	ui32	Count;
	ui32	Capacity;

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

	TArray( const T* data )
	{
		Item = (T*)data;
		Count = CountOfZeroEnding(data);
		Capacity = 0;
	}

	/**
	 * Constructs a static array.
	 */
	TArray( T* data, int dataCount)
	{
		Item = data;
		Count = dataCount;
		Capacity = 0;
	}

	TArray( const TArray<T>& cpy )
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

	void DeletePointers()
	{
		for (ui32 i=0;i<Count;i++)
		{
			delete Item[i];
		}
		Count = 0;
	}

	void RemoveBetween(ui32 startIndex, ui32 length)
	{
		if (startIndex >= Count)
			startIndex = Count-1;
		
		if (startIndex + length > Count)
			length = Count - startIndex;

		if (startIndex == Count-1 && length == 1)
		{
			RemoveLast();
			return;
		}

		int rstart = startIndex + length;
		int remaining = Count - rstart;
		for (int i=0;i<remaining;i++)
		{
			Item[startIndex+i] = Item[rstart+i];
		}

		Count -= length;
	}

	void Insert(T value,ui32 index)
	{
		if (Capacity <= Count) // TODO: make this a function named CheckCapacity
		{
			if (Capacity == 0) Capacity = 4;
			Allocate(Capacity<<1); // Multiply the cache
		}

		if (index >= Count)
		{
			Add(value);
			return;
		}

		int i = (int)Count;
		while( i-- >= (int)index)
		{
			Item[i+1] = Item[i];
		}

		Item[index] = value;
		Count++;
	}

	/**
	* Generic counter for zero ending array.
	*/
	static inline int CountOfZeroEnding(const T* _array)
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
				LowLevelException(SOURCENAME(0),__LINE__,0,"Impossible"); //Exception("Impossible"); // TODO: if it will be possible someday change this.
			}
		}
	}

	inline int IndexOf(T value)
	{
		for (ui32 i=0;i<Count;i++)
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

	inline void RemovePreserve(T value)
	{
		assert(Capacity != 0);

		int i = LastIndexOf(value);
		if (i != -1)
		{
			RemovePreserveAt(i);
		}
	}

	/**
	 * Removes and preserves the order.
	 */
	inline void RemovePreserveAt(int index)
	{
		assert(Capacity != 0);

		if (index == Count -1)
		{
			Count--;
			return;
		}

		int rem = Count - index;
		while(rem--)
		{
			Item[index] = Item[index+1];
			index++;
		}
		Count--;
	}

	/**
	 * Removes item and uses last item to replace it.
	 * WARNING: Breaks the order of items!
	 * TODO: Rename this function as RemoveFast change name of preserving.
	 */
	inline void Remove(T value)
	{
		assert(Capacity != 0);

		int i = LastIndexOf(value);
		if (i != -1)
		{
			RemoveAt(i);
		}
	}

	inline void Swap(ui32 srcIndex, ui32 dstIndex)
	{
		T tmp = Item[dstIndex];
		Item[dstIndex] = Item[srcIndex];
		Item[srcIndex] = tmp;
	}

	/**
	 * Removes item and uses last item to replace it.
	 * WARNING: Breaks the order of items!
	 */
	inline void RemoveAt(int index)
	{
		assert(Capacity != 0);

		if (index == Count -1)
		{
			Count--;
			return;
		}
		Item[index] = Item[--Count];
	}

	inline void RemoveLast()
	{
		if (Count)
		{
			Count--;
		}
		return;
	}

	void Clear()
	{
		assert(Capacity != 0);

		memset(Item,NULL, sizeof(T) * Count);
		Count = 0;
	}

	inline void	Add(const T& value)
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
		if (Item && Capacity > 0) // if capacity is 0 then this means data is static
		{
			delete [] Item;
		}
	}

	inline T& operator [] (ui32 index)
	{
		if ( index > Count )
		{
			LowLevelException(SOURCENAME(0),__LINE__,1,"Index out of bounds");
		}
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
	 * Gets last item and removes it from array.
	 */
	inline T Pop()
	{
		if (Count > 0)
		{
			T k = Item[Count-1];
			Count--;
			return k;
		}
		return NULL;
	}


	/**
	* Assigning of arrays will destroy other.
	*/
	inline TArray<T>& operator = (TArray<T>& other)
	{
		// TODO: USE SHARED STUFF LIKE TSTRING HERE
		Free();
		Item = other.Item;
		Count = other.Count;
		Capacity = other.Capacity;
		other.Item = 0;
		other.Capacity = 0;
		other.Count = 0;
		return *this;
	}

	void Allocate(int newsize)
	{
		if (newsize == 0)
		{
			if (Item)
			{
				delete [] Item;
			}

			Capacity = 0;
			Item = 0;
			Count = 0;
			
			LowLevelException(SOURCENAME(0),__LINE__,2,"Allocate 0 for array is unnecessary maybe bug?");
			//throw Exception(0, __LINE__ ,3);//"Index out of bounds");
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

template <class T>
class TArrayEnumerator: public TEnumerator<T>
{
public:
	TArray<T>& CurrentArray;
	ui32 Index;

	inline TArrayEnumerator( TArray<T>& curArray ):CurrentArray(curArray)
	{
		Reset();
	}

	inline void Reset()
	{
		Index = 0;
	}

	inline bool MoveNext()
	{
		if (Index >= CurrentArray.Count)
			return false;

		Current = CurrentArray.Item[Index];
		Index++;
		return true;
	}
};

template <class T>
class TArrayEnumeratorReverse: public TEnumerator<T>
{
public:
	TArray<T>& CurrentArray;
	ui32 Index;

	TArrayEnumeratorReverse( TArray<T>& curArray ):CurrentArray(curArray)
	{
		Reset();
	}

	inline void Reset()
	{
		Index = CurrentArray.Count - 1;
	}

	inline bool MoveNext()
	{
		if (Index == (ui32)-1)
			return false;

		Current = CurrentArray.Item[Index];
		Index--;
		return true;
	}

};

#endif