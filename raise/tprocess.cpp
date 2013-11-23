#include "stdafx.h"

#ifdef WIN32

#include "tprocess.h"
#include "texception.h"
#include "tplatform.h"



#include <Psapi.h>

TString TProcess::GetModulePath()
{
	ch16 Temp[4096];
	Temp[0] = 0;
	GetModuleFileNameExW(processHandle,0,Temp,4096);
	return Platform.RaiseString(Temp);
}

void TProcess::CloseProcessHandle()
{
	if (processHandle)
	{
		CloseHandle(processHandle);
	}
}

void TProcess::OpenProcessHandle()
{
	OpenProcessHandle(PROCESS_ALL_ACCESS);
}

void TProcess::OpenProcessHandle( bool memoryRead, bool memoryWrite, bool debug )
{
	if (memoryRead && memoryWrite && debug)
	{
		OpenProcessHandle();
		return;
	}

	ui32 access = 0;
	if (memoryRead)
	{
		access |= PROCESS_VM_READ;
	}

	if (memoryWrite)
	{
		access |= PROCESS_VM_WRITE;
	}

	OpenProcessHandle(access);
}

void TProcess::OpenProcessHandle( ui32 access )
{
	processHandle = OpenProcess(access,0,processID);
	if (!processHandle)
	{
		throw Exception("Can't open process");
	}

	Memory.InitializeMemory(this);
	Debug.InitializeDebug(this);
	Memory2.InitializeMemory2(this);
}

ui32 TProcessMemory::SearchAll( ui32 start, ui32 end, const void* needle, int length, TArray<ui32>& result )
{
	int foundCount = 0;
	for (ui32 addr=start;addr < end; addr += Buffer.Capacity)
	{
		ui32 readLength = end - addr;
		if (readLength > Buffer.Capacity)
		{
			readLength = Buffer.Capacity;
		}
		ui32 readed = Read(addr,Buffer.Data,readLength);

		if (readed != 0)
		{
			Buffer.SetVirtual(addr,addr+readed);
			Buffer.Index = readed;
			ui32 found = 0;
			while (1)
			{
				found = Buffer.VirtualSearch((byte*)needle + found,length);
				if (found == NFOUND) break;
				result.Add(found);
				found++;
				foundCount++;
			}
			
		}

		addr -= length-1;
	}
	return foundCount;
}


ui32 TProcessMemory::Search( ui32 start,ui32 end, const void* needle, int length )
{
	for (ui32 addr=start;addr < end; addr += Buffer.Capacity)
	{
		ui32 readLength = end - addr;
		if (readLength > Buffer.Capacity)
		{
			readLength = Buffer.Capacity;
		}
		ui32 readed = Read(addr,Buffer.Data,readLength);
		
		if (readed != 0)
		{
			Buffer.SetVirtual(addr,addr+readed);
			Buffer.Index = readed;
			ui32 found = Buffer.VirtualSearch((byte*)needle,length);
			if (found != NFOUND) return found;
		}
		
		addr -= length-1;
	}
	return NFOUND;
}

ui32 TProcessMemory::SearchPattern( ui32 start,ui32 end, const void* needle, const byte* pattern, int length )
{
	for (ui32 addr=start;addr < end; addr += Buffer.Capacity)
	{
		if (FillBuffer(addr) != 0) // 0 bytes readed at that section
		{
			ui32 found = Buffer.VirtualSearchPattern((byte*)needle,pattern,length);
			if (found != NFOUND) return found;
		}
		addr -= length-1;
	}
	return NFOUND;
}

void TProcessMemory::InitializeMemory( TProcess* pprocess )
{
	InitializeFrom(*pprocess);
	Process = pprocess;

	Buffer.Allocate(64 * 1024);
	MainRegion.Initialize("Main Memory Region",0x00400000,0x7FFFFFFF,this);
}

void TProcessDebug::InitializeDebug( TProcess* pprocess )
{
	InitializeFrom(*pprocess);
	Process = pprocess;
}

ui32 TProcessMemory::GetPointerOffset( ui32 basePtr, ui32 offset1 )
{
	ui32 cur = ReadDWord(basePtr);
	if (cur == 0)
	{
		throw Exception("Base address points to null", 1);
	}

	cur += offset1;
	return cur;
}

int TProcessMemory::GetPointer( TProcessPointerMultilevel& p )
{
	int curLevel = 0;

	ui32 currentPtr = p.BasePointer;

	for (int i=0;i<p.Offsets.Count;i++)
	{
		currentPtr = ReadDWord(currentPtr);
		if (currentPtr == 0)
		{
			p.RealPointer = 0;
			return -curLevel;
		}
		currentPtr += p.Offsets[i].Offset;
		curLevel++;
	}

	p.RealPointer = currentPtr;
	return 1;
}


#endif

