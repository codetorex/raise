#include "stdafx.h"


#ifdef WIN32
#include "tprocess.h"

ui32 TProcessMemoryRegion::FindString( const String& src )
{
	if (src.Length != src.ByteLength)
	{
		throw Exception("String should be ASCII (no characters over 127)");
	}
	return Memory->Search(StartAddress,EndAddress,src.Data,src.Length);
}

ui32 TProcessMemoryRegion::FindXref( ui32 addr)
{
	return Memory->Search(StartAddress,EndAddress,&addr,4);
}

ui32 TProcessMemoryRegion::FindFunctionBegin( ui32 addr ,int intCount)
{
	int counter = 0;

	byte fncBytes[1024];
	Memory->Read(addr-1024,fncBytes,1024);
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

ui32 TProcessMemoryRegion::FindFunctionFromString( const String& src )
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

ui32 TProcessMemoryRegion::FindFunctionFromString( const String& src, byte firstByte)
{
	ui32 fncAddr = FindFunctionFromString(src);
	byte RealFirstByte = Memory->ReadByte(fncAddr);
	if (RealFirstByte != firstByte )
	{
		throw Exception("Function begin check failed");
	}
	return fncAddr;
}

void TProcessMemoryRegion::Initialize( const String& _name, ui32 _start,ui32 _end,TProcessMemory* _prn )
{
	ModuleName = _name;
	StartAddress = _start;
	EndAddress = _end;
	EntryPointAddress = 0x00400000;
	Size = EndAddress - StartAddress;
	Memory = _prn;
}

TProcessMemoryRegion::TProcessMemoryRegion( const String& _name, const String& _file,ui32 _start,ui32 _end,TProcessMemory* _prn )
{
	Initialize(_name,_start,_end,_prn);
	FileName = _file;
}

TProcessMemoryRegion::TProcessMemoryRegion( const String& _name, ui32 _start,ui32 _end,TProcessMemory* _prn )
{
	Initialize(_name,_start,_end,_prn);
}

TProcessMemoryRegion::TProcessMemoryRegion()
{
	StartAddress = 0;
	EndAddress = 0;
	EntryPointAddress = 0;
	Size = 0;
}

TProcessMemoryRegion::TProcessMemoryRegion( ui32 _start,ui32 _end,TProcessMemory* prn )
{
	StartAddress = _start;
	EndAddress = _end;
	Memory = prn;

	EntryPointAddress = 0;
	Size = EndAddress - StartAddress;
}

ui32 TProcessMemoryRegion::Search( const void* needle, int length )
{
	return Memory->Search(StartAddress,EndAddress,needle,length);
}

ui32 TProcessMemoryRegion::SearchPattern( const void* needle, const byte* pattern, int length )
{
	return Memory->SearchPattern(StartAddress,EndAddress,needle,pattern,length);
}

ui32 TProcessMemoryRegion::SearchAll( const void* needle, int length, Array<ui32>& result )
{
	return Memory->SearchAll(StartAddress,EndAddress, needle, length, result);
}


#endif