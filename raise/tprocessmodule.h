#ifndef TPROCESSMODULE_H
#define TPROCESSMODULE_H

#ifdef WIN32

#include "raisetypes.h"

class TProcessHack;

class TProcessMemoryRegion
{
public:
	str8 ModuleName;
	str8 FileName;

	//dword BaseAddress; ?
	dword StartAddress;
	dword EndAddress;
	dword EntryPointAddress;
	dword ModuleMemorySize;

	TProcessHack* Parent;

	TProcessMemoryRegion();
	TProcessMemoryRegion( dword _start,dword _end,TProcessHack* prn);
	TProcessMemoryRegion( const str8& _name, dword _start,dword _end,TProcessHack* _prn);
	TProcessMemoryRegion( const str8& _name, const str8& _file,dword _start,dword _end,TProcessHack* _prn);

	void Initialize(const str8& _name, dword _start,dword _end,TProcessHack* _prn);

	dword FindString(const str8& src);
	dword FindString(const str16& src);
	dword FindXref(dword addr);
	dword FindFunctionBegin(dword addr,int intCount = 3);
	dword FindFunctionFromString(const str8& src);
	dword FindFunctionFromString(const str8& src, byte firstByte);

	dword Search( const void* needle, int length);
	dword SearchPattern(const void* needle, const byte* pattern, int length);
};

typedef TProcessMemoryRegion TProcessModule;

#endif
#endif