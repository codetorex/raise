#include "stdafx.h"

#ifdef WIN32

#include "tprocess.h"
#include "texception.h"
#include "twintools.h"



#include <Psapi.h>

TString TProcess::GetModulePath()
{
	ch16 Temp[4096];
	Temp[0] = 0;
	GetModuleFileNameExW(processHandle,0,Temp,4096);
	return TWinTools::RaiseString(Temp);
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
	processHandle = OpenProcess(PROCESS_ALL_ACCESS,0,processID);
	if (!processHandle)
	{
		throw Exception("Can't open process");
	}

	Memory.InitializeMemory(this);
	Debug.InitializeDebug(this);
}

ui32 TProcessMemory::Search( ui32 start,ui32 end, const void* needle, int length )
{
	for (ui32 addr=start;addr < end; addr += Buffer.Capacity)
	{
		if (FillBuffer(addr) != 0) // 0 bytes readed at that section
		{
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

	Buffer.Allocate(4 * 1024);
	MainRegion.Initialize("Main Memory Region",0x00400000,0x7FFFFFFF,this);
}

void TProcessDebug::InitializeDebug( TProcess* pprocess )
{
	InitializeFrom(*pprocess);
	Process = pprocess;
}

#endif

