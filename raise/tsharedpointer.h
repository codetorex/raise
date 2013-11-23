#ifndef TSHAREDPOINTER_H
#define TSHAREDPOINTER_H

#include "raisetypes.h"

namespace raise
{

class ReferenceCounter
{
public:
	ui32 Count;
	
	inline void Acquire()
	{
		Count++;
	}
	
	inline ui32 Release()
	{
		return (--Count);
	}
};

template <class T, bool isArray = false>
class SharedPointer
{
public:
	/// Counter will be nullptr when Pointer is static data
	ReferenceCounter* Counter;
	T* Pointer;
	
	inline SharedPointer()
	{
		Counter = 0;
		Pointer = 0;
	}
	
	inline SharedPointer( T* value )
	{
		Pointer = 0;
		*this = value;
	}
	
	inline SharedPointer( const SharedPointer<T>& other )
	{
		Pointer = 0;
		*this = other;
	}
	
	/**
	 * If this pointer going to be edited this function ensures it is not going to be used by other objects.
     * @return Returns a new shared pointer if it is referenced by other sources. Or returns same pointer if it is acquired only one time.
     */
	inline SharedPointer& AcquireUnique()
	{
		if ( Counter->Count == 1 )
		{
			return *this;
		}
		// Create new shared pointer
	}
	
	inline void Acquire()
	{
		if ( Counter != nullptr )
		{
			Counter->Acquire();
		}
	}
	
	inline void Release()
	{
		if ( Counter != nullptr )
		{
			if( Counter->Release() == 0 )
			{
				if ( isArray )
				{
					delete [] Pointer;
				}
				else
				{
					delete Pointer;
				}
				delete Counter;
			}
		}
		
		Pointer = 0;
		Counter = 0;
	}
	
	inline ~SharedPointer()
	{
		Release();
	}
	

	
	inline SharedPointer& operator = ( const T* ptr )
	{
		Pointer = (T*)ptr;
		Counter = nullptr;
		return *this;
	}
	
	inline SharedPointer& operator = ( T* ptr )
	{
		if (Pointer == ptr)
		{
			return *this;
		}
		
		if (Pointer != 0 ) Release();
		
		Pointer = ptr;
		Counter = new ReferenceCounter();
		Acquire();
		return *this;
	}
	
	inline SharedPointer& operator = ( const SharedPointer& other )
	{
		Pointer = other.Pointer;
		Counter = other.Counter;
		Acquire();
		return *this;
	}

	inline T& operator* () const
	{
		return *Pointer;
	}
	
	inline T* operator-> () const
	{
		return Pointer;
	}
};

}
#endif