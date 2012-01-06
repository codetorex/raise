#ifndef TOBJECTPOOL_H
#define TOBJECTPOOL_H

#include "raisetypes.h"
#include "tarray.h"

// TODO: test tobject pool

/**
 * Purpose of this class is having preallocated pool of objects.
 * Needed for networking stuff but can be used elsewhere when needed.
 * Objects have to be simple stuff. e.g. having no constructor
 */

template <class T>
class TObjectPool
{
public:
	ui32 ObjectLength;
	ui32 FreeCount;
	ui32 AllocatedCount;
	ui32 TotalCount;

	TArray<T*> ObjectArray;

	TArray<void*> Allocations;

	TObjectPool(ui32 initialSize = 0)
	{
		ObjectLength = sizeof(T);
		
		if (initialSize > 0)
		{
			AllocateObjects(initialSize);
		}
	}

	~TObjectPool()
	{
		for (ui32 i=0;i<Allocations.Count;i++)
		{
			delete Allocations.Item[i];
		}
		Allocations.Clear();
	}

	/**
	 * Gets an object from pool.
	 */
	inline T* PopObject()
	{
		T* objectToReturn = ObjectArray.GetLast();
		if (objectToReturn)
		{
			ObjectArray.RemoveLast();
		}
		return objectToReturn;
	}

	/**
	 * Returns the object to the pool.
	 */
	inline void PushObject(T* Object)
	{
		ObjectArray.UnsafeAdd(Object);
	}

	/**
	 * Preallocates number of objects.
	 */
	void AllocateObjects(ui32 Count)
	{
		ui32 requiredMemory = Count * ObjectLength;
		void* currentAllocation = new byte [ requiredMemory ];

		Allocations.Add(currentAllocation);
		ObjectArray.Allocate(Count + ObjectArray.Count + 8);

		void* currentObject = currentAllocation;

		for (ui32 i = 0;i<Count;i++)
		{
			PushObject((T*)currentObject);
			currentObject += ObjectLength;
		}
	}

};



#endif