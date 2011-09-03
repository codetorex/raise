#ifndef TPROCESSMODULE_H
#define TPROCESSMODULE_H

#ifdef WIN32

#include "raisetypes.h"

class TProcessHack;

class TProcessMemoryRegion
{
public:
	TString ModuleName;
	TString FileName;

	//dword BaseAddress; ?
	dword StartAddress;
	dword EndAddress;
	dword EntryPointAddress;
	dword ModuleMemorySize;

	TProcessHack* Parent;

	TProcessMemoryRegion();
	TProcessMemoryRegion( dword _start,dword _end,TProcessHack* prn);
	TProcessMemoryRegion( const TString& _name, dword _start,dword _end,TProcessHack* _prn);
	TProcessMemoryRegion( const TString& _name, const TString& _file,dword _start,dword _end,TProcessHack* _prn);

	void Initialize(const TString& _name, dword _start,dword _end,TProcessHack* _prn);

	/**
	 * Searchs an ascii string.
	 */
	dword FindString(const TString& src);

	/**
	 * Finds an unicode (utf-16) string.
	 */
	dword FindUnicodeString(const TString& src);
	dword FindXref(dword addr);
	dword FindFunctionBegin(dword addr,int intCount = 3);
	dword FindFunctionFromString(const TString& src);
	dword FindFunctionFromString(const TString& src, byte firstByte);

	dword Search( const void* needle, int length);
	dword SearchPattern(const void* needle, const byte* pattern, int length);
};

typedef TProcessMemoryRegion TProcessModule;

#endif
#endif