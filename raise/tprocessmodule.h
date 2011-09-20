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
	ui32 StartAddress;
	ui32 EndAddress;
	ui32 EntryPointAddress;
	ui32 ModuleMemorySize;

	TProcessHack* Parent;

	TProcessMemoryRegion();
	TProcessMemoryRegion( ui32 _start,ui32 _end,TProcessHack* prn);
	TProcessMemoryRegion( const TString& _name, ui32 _start,ui32 _end,TProcessHack* _prn);
	TProcessMemoryRegion( const TString& _name, const TString& _file,ui32 _start,ui32 _end,TProcessHack* _prn);

	void Initialize(const TString& _name, ui32 _start,ui32 _end,TProcessHack* _prn);

	/**
	 * Searchs an ascii string.
	 */
	ui32 FindString(const TString& src);

	/**
	 * Finds an unicode (utf-16) string.
	 */
	ui32 FindUnicodeString(const TString& src);
	ui32 FindXref(ui32 addr);
	ui32 FindFunctionBegin(ui32 addr,int intCount = 3);
	ui32 FindFunctionFromString(const TString& src);
	ui32 FindFunctionFromString(const TString& src, byte firstByte);

	ui32 Search( const void* needle, int length);
	ui32 SearchPattern(const void* needle, const byte* pattern, int length);
};

typedef TProcessMemoryRegion TProcessModule;

#endif
#endif