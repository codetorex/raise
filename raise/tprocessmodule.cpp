#include "stdafx.h"
#include "tprocessdebug.h"
#include "tprocessmodule.h"

dword TProcessMemoryRegion::FindString( const str8& src )
{
	return Parent->Search(StartAddress,EndAddress,src.Chars,src.Length);
}

dword TProcessMemoryRegion::FindXref( dword addr)
{
	return Parent->Search(StartAddress,EndAddress,&addr,4);
}

dword TProcessMemoryRegion::FindFunctionBegin( dword addr ,int intCount)
{
	int counter = 0;

	byte fncBytes[1024];
	Parent->Read(addr-1024,fncBytes,1024);
	int i = 1024;
	while(i--)
	{
		if (fncBytes[i] == 0xCC)
		{
			counter++;

			if (counter == intCount)
			{
				return (addr - (1024-i)) + intCount;
			}
		}
		else
		{
			counter = 0;
		}
	}

	return NFOUND;
}

dword TProcessMemoryRegion::FindFunctionFromString( const str8& src )
{
	dword strAddr = FindString(src);
	if (strAddr == NFOUND)
	{
		throw Exception("String not found");
	}

	dword xrefAddr = FindXref(strAddr);
	if (xrefAddr == NFOUND)
	{
		throw Exception("Xref not found (string not used in code?)");
	}

	dword fncBegin = FindFunctionBegin(xrefAddr);
	if (fncBegin == NFOUND)
	{
		throw Exception("Function begining not found");
	}
	return fncBegin;
}

dword TProcessMemoryRegion::FindFunctionFromString( const str8& src, byte firstByte)
{
	dword fncAddr = FindFunctionFromString(src);
	byte RealFirstByte = Parent->ReadByte(fncAddr);
	if (RealFirstByte != firstByte )
	{
		throw Exception("Function begin check failed");
	}
	return fncAddr;
}

void TProcessMemoryRegion::Initialize( const str8& _name, dword _start,dword _end,TProcessHack* _prn )
{
	ModuleName = _name;
	StartAddress = _start;
	EndAddress = _end;
	EntryPointAddress = 0x00400000;
	ModuleMemorySize = EndAddress - StartAddress;
	Parent = _prn;
}

TProcessMemoryRegion::TProcessMemoryRegion( const str8& _name, const str8& _file,dword _start,dword _end,TProcessHack* _prn )
{
	Initialize(_name,_start,_end,_prn);
	FileName = _file;
}

TProcessMemoryRegion::TProcessMemoryRegion( const str8& _name, dword _start,dword _end,TProcessHack* _prn )
{
	Initialize(_name,_start,_end,_prn);
}

TProcessMemoryRegion::TProcessMemoryRegion()
{
	StartAddress = 0;
	EndAddress = 0;
	EntryPointAddress = 0;
	ModuleMemorySize = 0;
}

TProcessMemoryRegion::TProcessMemoryRegion( dword _start,dword _end,TProcessHack* prn )
{
	StartAddress = _start;
	EndAddress = _end;
	Parent = prn;

	EntryPointAddress = 0;
	ModuleMemorySize = 0;
}

dword TProcessMemoryRegion::Search( const void* needle, int length )
{
	return Parent->Search(StartAddress,EndAddress,needle,length);
}

dword TProcessMemoryRegion::SearchPattern( const void* needle, const byte* pattern, int length )
{
	return Parent->SearchPattern(StartAddress,EndAddress,needle,pattern,length);
}