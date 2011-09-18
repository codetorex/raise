#ifndef TMEMORY_H
#define TMEMORY_H

#include "raisetypes.h"
#include "tlinkedlist.h"
#include "tbitset.h"

class TMemoryAllocator
{
public:
	virtual void* Allocate(dword size) = 0;
	virtual int Deallocate(void* ptr) = 0;
	virtual int Reallocate(void* ptr, dword newsize) = 0;
	virtual bool IsFull() = 0;
};


// Influenced by double-entry bookkeeping system.
/*class TAllocationEntry
{
public:
	TAllocationEntry* next;
	void* start;
	dword length;

	void CutFromBeginning(dword length);
};

class TAllocationBook;

class TAllocationAccount
{
public:
	TAllocationBook* Book;

	TAllocationEntry* firstEntry;
	TAllocationEntry* lastEntry;

	TAllocationEntry* firstEmptyEntry;
	TAllocationEntry* lastEmptyEntry;

	int entryCount;
	int entryFree;

	TAllocationAccount();

	void AddEntry(void* start,dword length); // must insert sorted.
	void RemoveEntry(void* start);
	void ResizeEntry(void* start,dword newlength);

	void Initialize(TAllocationEntry* newBook,int bookLength);
	void Extend(TAllocationEntry* newBook,int bookLength);
};

class TAllocationBook
{
public:
	TAllocationAccount Debit; // used memory account
	TAllocationAccount Credit; // free memory account
	dword Balance; // free memory amount
	bool Full;

	TAllocationBook();

	void DepositCash(void* start,dword length); // will goes to free memory account
	void DepositDebit(void* start); // will removes from debit and adds to credit
	void* WithdrawAmount(dword length); // will goes to debit, and removes from credit
};

class TMemoryBookkeeperAllocator: TMemoryAllocator
{
public:
	TMemoryPool*	Pool;
	TAllocationBook	Book;

	void*			Allocate(dword size);
	int				Deallocate(void* ptr);
	int				Reallocate(void* ptr, dword newsize);
	bool			IsFull();
};*/



/*class TMemoryLinearAllocator: public TMemoryAllocator
{
	TMemoryPool*	Pool;

	void*			Allocate(dword size);
	int				Deallocate(void* ptr);
	int				Reallocate(void* ptr, dword newsize);
};*/


class TMemoryBitmapAllocator: public TMemoryAllocator
{
public:


	TMemoryPool*	Pool;
	TBitset*		BitArray;
	int				BytesPerBit;

	int				firstFreeBlock;
	int				totalBlocks;
	int				emptyBlocks;

	int				failedToAllocate;

	TMemoryBitmapAllocator(TMemoryPool* pool,int bpb);

	void*	operator new(size_t,TMemoryPool* pool);
	void	operator delete(void* obj,TMemoryPool* pool);

	void*			Allocate(dword size);
	int				Deallocate(void* ptr);
	int				Reallocate(void* ptr, dword newsize);
	bool			IsFull();

private:
	int				CalculateRequiredBlocks(dword length);
};

// Will have size less than 4gbs for using over 4gb use multiple pools
class TMemoryPool: public TListNode<TMemoryPool*>
{
public:
	void* PoolStart;	// beginning of memory pool

	dword MemSize;
	dword MemUsed;
	dword MemFree;

	TMemoryPool();
	
	void*	operator new(size_t,dword rsize);
	void	operator delete(void* obj,dword rsize);

	TMemoryAllocator* Allocator;

	//void UseBookkeperAllocator();
	void UseBitmapAllocator(int bytesPerBlock);
	
	void* InitializeAllocate(dword size);
};



class TMemoryManager: public TList<TMemoryPool*>, public TMemoryAllocator
{
public:
	bool			AllowNewPools;
	
	TMemoryPool*	CreateNewPool(dword size);

	void*			Allocate(dword size);
	int				Deallocate(void* ptr);
	int				Reallocate(void* ptr, dword newsize);
	bool			IsFull();

};



#endif