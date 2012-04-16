#ifndef TOBJECTPOOL_H
#define TOBJECTPOOL_H

#include "raisetypes.h"
#include "tarray.h"

// TODO: test tobject pool
template <class T>
class TBasePool
{
public:
	ui32 ObjectLength;
	ui32 TotalObjects;

	TArray<T*> ObjectArray;
	TArray<T*> Allocations;

	TBasePool()
	{
		TotalObjects = 0;
	}

	inline ui32 GetFreeObjects()
	{
		return ObjectArray.Count;
	}

	inline ui32 GetAllocatedObjects()
	{
		return TotalObjects - ObjectArray.Count;
	}

	inline ui32 GetTotalObjects()
	{
		return TotalObjects;
	}

	
	~TBasePool()
	{
		this->Allocations.DeletePointers();
	}

	/**
	 * Gets an object from pool.
	 */
	inline T* AcquireObject()
	{
		if (this->ObjectArray.Count)
		{
			return ObjectArray.Item[--ObjectArray.Count];
		}
		
		DoubleObjectCount(); // LOG this situation?
		return AcquireObject();
	}

	/**
	 * Returns the object to the pool.
	 */
	inline void ReleaseObject(T* Object)
	{
		this->ObjectArray.UnsafeAdd(Object);
	}

	/**
	 * Doubles allocated object count.
	 */
	inline void DoubleObjectCount()
	{
		AllocateObjects(TotalObjects);
	}

	virtual void AllocateObjects(ui32 Count) = 0;
};

/**
 * Purpose of this class is having preallocated pool of objects.
 */
template <class T>
class TObjectPool: public TBasePool<T>
{
public:
	TObjectPool(ui32 initialSize = 0)
	{
		this->ObjectLength = sizeof(T);

		if (initialSize > 0)
		{
			AllocateObjects(initialSize);
		}
	}

	/**
	 * Preallocates number of objects.
	 */
	void AllocateObjects(ui32 Count)
	{
		T* currentAllocation = new T [ Count ];
		Allocations.Add(currentAllocation);

		this->ObjectArray.Allocate(Count + TotalObjects);

		for (ui32 i = 0;i<Count;i++)
		{
			ReleaseObject( &currentAllocation[i] );
		}

		TotalObjects += Count;
	}
};

class TMemoryPool: public TBasePool<byte>
{
public:

	TMemoryPool( ui32 chunkLength , ui32 initialSize)
	{
		this->ObjectLength = chunkLength;

		if (initialSize > 0)
		{
			AllocateObjects(initialSize);
		}
	}

	/**
	 * Use this function to import memory location of a stack location or object location.
	 */
	void ImportStaticMemory(byte* pMemory, int length)
	{
		ui32 objectCount = length / ObjectLength;

		if (objectCount <= 0) return;

		int i = objectCount;
		while(i--)
		{
			ReleaseObject(pMemory);
			pMemory += ObjectLength;
		}

		TotalObjects += objectCount;
	}

	void AllocateObjects(ui32 Count)
	{
		ui32 requiredMemory = Count * ObjectLength;
		byte* currentAllocation = new byte [ requiredMemory ];
		
		Allocations.Add(currentAllocation);
		ObjectArray.Allocate( TotalObjects + Count );

		byte* currentObject = currentAllocation;
		for ( ui32 i=0; i<Count ; i++)
		{
			ReleaseObject(currentObject);
			currentObject += ObjectLength;
		}

		TotalObjects += Count;
	}
};



#endif