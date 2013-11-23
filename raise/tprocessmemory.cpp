#include "stdafx.h"
#include "tprocess.h"
#include "tplatform.h"

#include <psapi.h>

void TProcessMemory2::UpdateMemoryRegions()
{
	Regions.Free();
	Regions.Clear();

	LPCVOID currentAddress = NULL;
	CommitSize = 0;
	CommitCount = 0;
	TotalSize = 0;

	while (1)
	{
		MEMORY_BASIC_INFORMATION bi;
		SIZE_T memSize = VirtualQueryEx(Process->processHandle, currentAddress, &bi, sizeof(bi));

		if (memSize ==  0)
		{
			DWORD lasterror = GetLastError();
			if (lasterror != ERROR_SUCCESS && lasterror != ERROR_INVALID_PARAMETER)
			{
				throw OSException(Platform.GetErrorDescription(lasterror), lasterror);
			}

			break;
		}

		TProcessMemoryRegion2* rg = new TProcessMemoryRegion2((ui64)bi.BaseAddress, bi.RegionSize, this);
		rg->Type = bi.Type;
		rg->State = bi.State;
		rg->Protect = bi.Protect;
		Regions.Add(rg);

		if (rg->State == MEM_COMMIT)
		{
			CommitSize += rg->Length;
			CommitCount++;
		}
		TotalSize += rg->Length;

		currentAddress = (LPCVOID)rg->End;
	}
}

int TProcessMemory2::FindAll( ui64 start, ui64 end, const void* needle, int length, TArray< ui64 >& result )
{
	// allocate in stack
	byte cache[65536];
	int foundCount = 0;

	for (ui64 src = start; src < end; src += 65536)
	{
		int readed = Read(src, cache, 65536);
		if (readed > 0)
		{
			int found = 0;
			while (true)
			{
				found = MemoryDriver::Search(cache + found, readed - found, needle, length);
				if (found != -1)
				{
					result.Add(src + found);
					found++;
					foundCount++;
				}
				else
				{
					break;
				}
			}
		}

		src -= length-1;
	}
	return foundCount;
}

int TProcessMemory2::FindAll( const void* needle, int length, TArray< ui64 >& result )
{
	int foundCount = 0;
	for (int i=0;i< Regions.Count;i++)
	{
		TProcessMemoryRegion2* curRegion = Regions.Item[i];
		if (curRegion->State == MEM_COMMIT)
		{
			foundCount += FindAll(curRegion->Start, curRegion->End, needle,length,result);
		}
	}
	return foundCount;
}

int TProcessMemory2::FindAll( ui64 start, ui64 end, float needle, float epsilon, TArray< ui64 >& result )
{
	// allocate in stack
	byte cache[65536];
	int foundCount = 0;

	for (ui64 src = start; src < end; src += 65536)
	{
		int readed = Read(src, cache, 65536);
		if (readed > 0)
		{
			int found = 0;
			int searchStart = 0;
			while (true)
			{
				found = MemoryDriver::Search(cache + searchStart, readed - searchStart, needle, epsilon);
				if (found != -1)
				{
					result.Add(src + searchStart + found);
					searchStart = searchStart + found + sizeof(float);
					foundCount++;
				}
				else
				{
					break;
				}
			}
		}

		src -= sizeof(float)-1;
	}
	return foundCount;
}

int TProcessMemory2::FindAll( float needle, float epsilon, TArray< ui64 >& result )
{
	int foundCount = 0;
	for (int i=0;i< Regions.Count;i++)
	{
		TProcessMemoryRegion2* curRegion = Regions.Item[i];
		if (curRegion->State == MEM_COMMIT)
		{
			foundCount += FindAll(curRegion->Start, curRegion->End, needle,epsilon,result);
		}
	}
	return foundCount;
}

void TProcessMemory2::InitializeMemory2( TProcess* process )
{
	this->InitializeFrom( *process );
	this->Process = process;
}

#include "tconsole.h"
#include "tstringformat.h"

void TProcessMemory2::UpdateModules()
{
	Modules.Free();
	Modules.Clear();

	HMODULE hMods[1024];
	DWORD cbNeeded;
	if( EnumProcessModules(processHandle, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			TCHAR szModName[MAX_PATH];

			// Get the full path to the module's file.

			int modLength = GetModuleFileNameEx( processHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR));
			if ( modLength )
			{
				// Print the module name and handle value.

				TProcessMemoryRegion2* newMod = new TProcessMemoryRegion2();
				newMod->Path = Platform.SystemToRaiseString((byte*)szModName, modLength);
				newMod->Start = (ui64)hMods[i];
				Modules.Add(newMod);
			}
		}
	}
}

