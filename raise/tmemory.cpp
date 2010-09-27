#include "stdafx.h"
#include "tmemory.h"

#include <fstream>

TMemoryBitmapAllocator::TMemoryBitmapAllocator( TMemoryPool* pool,int bpb )
{
	this->Pool = pool;
	BytesPerBit = bpb;

	dword memRem = pool->MemSize % bpb;
	dword memQuo = (pool->MemSize - memRem) / bpb;
	dword reqBits = memQuo;
	if (memRem > 0)
		reqBits++;
	
	dword reqDwords = TBitset::CalculateDwordLengthFromBitLength(reqBits);
	totalBlocks = reqBits;

	BitArray = (TBitset*)pool->InitializeAllocate(sizeof(TBitset));
	dword* dArray = (dword*)pool->InitializeAllocate(reqDwords * sizeof(dword));
	*BitArray = TBitset(dArray,reqBits);
	dword alreadyFullBlocks = CalculateRequiredBlocks(pool->MemUsed); // Used by InitializeAllocate funcs.
	emptyBlocks = totalBlocks - alreadyFullBlocks;
	BitArray->FillBits(0, alreadyFullBlocks ,true);
	firstFreeBlock = (int)alreadyFullBlocks;
}

int TMemoryBitmapAllocator::CalculateRequiredBlocks( dword length )
{
	dword bitRem = length % BytesPerBit;
	dword bitQuo = (length - bitRem) / BytesPerBit;
	int bitReq = (int)bitQuo;
	if (bitRem > 0)
		bitReq++;
	return bitReq;
}

void* TMemoryBitmapAllocator::Allocate( dword size )
{
	int reqBlocks = CalculateRequiredBlocks(size + sizeof(int));
	if (failedToAllocate >= reqBlocks) return 0; // this way we avoid searching again...

	//TODO: implement "biggest empty block" variable and check for avoid search...

	int bstart = BitArray->SearchEmptyBits(reqBlocks,firstFreeBlock);

	if (bstart == -1)
	{
		failedToAllocate = reqBlocks;
		return 0;
	}

	if (bstart == firstFreeBlock)
		firstFreeBlock+=reqBlocks;

	failedToAllocate = 0;

	int* addr = (int*)&((byte*)Pool->PoolStart)[bstart * BytesPerBit];
	*addr = reqBlocks;
	emptyBlocks -= reqBlocks;
	BitArray->FillBits(bstart,reqBlocks,true);
	addr[1] = reqBlocks * BytesPerBit;
	return &addr[1];
}

int TMemoryBitmapAllocator::Deallocate( void* ptr )
{
	int* iPtr = (int*)ptr;
	int blocksToFreed = *(--iPtr);
	
	dword relPtr = (dword)((byte*)iPtr - (byte*)Pool->PoolStart);
	int startBlock = relPtr / BytesPerBit;
	BitArray->FillBits(startBlock,blocksToFreed,false);
	emptyBlocks+= blocksToFreed;
	return 0;
}

bool TMemoryBitmapAllocator::IsFull()
{
	return (emptyBlocks > 0);
}

int TMemoryBitmapAllocator::Reallocate( void* ptr, dword newsize )
{
	return 0;
}

void* TMemoryBitmapAllocator::operator new (size_t, TMemoryPool* pool)
{
	void* r = pool->InitializeAllocate(sizeof(TMemoryBitmapAllocator));
	return r;
}

void TMemoryBitmapAllocator::operator delete(void* obj,TMemoryPool* pool)
{

}

/*TAllocationAccount::TAllocationAccount()
{
	firstEntry = 0;
	firstEmptyEntry = 0;
	lastEntry = 0;
	lastEmptyEntry = 0;
	entryCount = 0;
	entryFree = 0;
}

TAllocationBook::TAllocationBook()
{
	Balance = 0;
	Full = true;
	Debit.Book = this;
	Credit.Book = this;
}

void TAllocationEntry::CutFromBeginning( dword length )
{
	start = ((byte*)start + length);
	this->length -= length;
}

void* TAllocationBook::WithdrawAmount(dword length)
{
	TAllocationEntry* curCredit = Credit.firstEntry;
	for (int i=0;i<Credit.entryCount;i++)
	{
		if (curCredit->length >= length)
		{
			break;
		}
		curCredit = curCredit->next;
	}

	if (curCredit == 0)
	{
		return 0;
	}

	Debit.AddEntry(curCredit->start,length);

	void* rt = curCredit->start;
	curCredit->CutFromBeginning(length);
	return rt;
}

void TAllocationBook::DepositCash( void* start,dword length )
{
	Credit.AddEntry(start,length);
}

void TAllocationBook::DepositDebit( void* start )
{
	
}


void* TMemoryBookkeeperAllocator::Allocate(dword size)
{
	if (Book.Full)
	{
		return 0; // book is full
	}

	return Book.WithdrawAmount(size);
}

int TMemoryBookkeeperAllocator::Deallocate( void* ptr )
{
	Book.DepositDebit( ptr );
	return 1;
}*/

TMemoryPool::TMemoryPool()
{
	Allocator = 0;
	PoolStart = this;
	MemUsed = 0;
	MemFree = MemSize;
	InitializeAllocate(sizeof(TMemoryPool));
}

void* TMemoryPool::operator new(size_t, dword rsize)
{
	void* pool = malloc(rsize);
	((TMemoryPool*)pool)->MemSize = rsize;
	return pool;
}

void TMemoryPool::operator delete(void* obj,dword rsize)
{
	free(obj);
}

void* TMemoryPool::InitializeAllocate( dword size )
{
	if (MemFree < size)
		return 0;
	
	MemFree -= size;
	void* rslt = &((byte*)PoolStart)[MemUsed];
	MemUsed += size;
	return rslt;
}

void TMemoryPool::UseBitmapAllocator(int bytesPerBlock)
{
	Allocator = new (this) TMemoryBitmapAllocator(this,bytesPerBlock);
}

TMemoryPool* TMemoryManager::CreateNewPool( dword size )
{
	TMemoryPool* mp = new(size) TMemoryPool();
	Add(mp);
	return mp;
}

void* TMemoryManager::Allocate( dword size )
{
	return 0;
}

int TMemoryManager::Deallocate( void* ptr )
{
	return 0;
}

int TMemoryManager::Reallocate( void* ptr, dword newsize )
{
	return 0;
}

bool TMemoryManager::IsFull()
{
	return false;
}