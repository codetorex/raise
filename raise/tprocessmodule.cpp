#include "stdafx.h"
#include "tprocessdebug.h"
#include "tprocessmodule.h"

ui32 TProcessMemoryRegion::FindString( const TString& src )
{
	if (src.Length != src.ByteLength)
	{
		throw Exception("String should be ASCII (no characters over 127)");
	}
	return Parent->Search(StartAddress,EndAddress,src.Data,src.Length);
}

ui32 TProcessMemoryRegion::FindXref( ui32 addr)
{
	return Parent->Search(StartAddress,EndAddress,&addr,4);
}

ui32 TProcessMemoryRegion::FindFunctionBegin( ui32 addr ,int intCount)
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

ui32 TProcessMemoryRegion::FindFunctionFromString( const TString& src )
{
	ui32 strAddr = FindString(src);
	if (strAddr == NFOUND)
	{
		throw Exception("String not found");
	}

	ui32 xrefAddr = FindXref(strAddr);
	if (xrefAddr == NFOUND)
	{
		throw Exception("Xref not found (string not used in code?)");
	}

	ui32 fncBegin = FindFunctionBegin(xrefAddr);
	if (fncBegin == NFOUND)
	{
		throw Exception("Function begining not found");
	}
	return fncBegin;
}

ui32 TProcessMemoryRegion::FindFunctionFromString( const TString& src, byte firstByte)
{
	ui32 fncAddr = FindFunctionFromString(src);
	byte RealFirstByte = Parent->ReadByte(fncAddr);
	if (RealFirstByte != firstByte )
	{
		throw Exception("Function begin check failed");
	}
	return fncAddr;
}

void TProcessMemoryRegion::Initialize( const TString& _name, ui32 _start,ui32 _end,TProcessHack* _prn )
{
	ModuleName = _name;
	StartAddress = _start;
	EndAddress = _end;
	EntryPointAddress = 0x00400000;
	ModuleMemorySize = EndAddress - StartAddress;
	Parent = _prn;
}

TProcessMemoryRegion::TProcessMemoryRegion( const TString& _name, const TString& _file,ui32 _start,ui32 _end,TProcessHack* _prn )
{
	Initialize(_name,_start,_end,_prn);
	FileName = _file;
}

TProcessMemoryRegion::TProcessMemoryRegion( const TString& _name, ui32 _start,ui32 _end,TProcessHack* _prn )
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

TProcessMemoryRegion::TProcessMemoryRegion( ui32 _start,ui32 _end,TProcessHack* prn )
{
	StartAddress = _start;
	EndAddress = _end;
	Parent = prn;

	EntryPointAddress = 0;
	ModuleMemorySize = 0;
}

ui32 TProcessMemoryRegion::Search( const void* needle, int length )
{
	return Parent->Search(StartAddress,EndAddress,needle,length);
}

ui32 TProcessMemoryRegion::SearchPattern( const void* needle, const byte* pattern, int length )
{
	return Parent->SearchPattern(StartAddress,EndAddress,needle,pattern,length);
}