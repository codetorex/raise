#include "stdafx.h"

#ifdef WIN32

#include "tpewin32.h"
#include "tplatform.h"

void TPortableExecutableWin32::LoadFromLibraryName( const String& libraryName )
{
	ch16 tmp[4096];
	Platform.RaiseToSystemString(libraryName,(byte*)tmp,sizeof(tmp));


	HMODULE mod = LoadLibraryW(tmp);

	if (mod == 0)
	{
		throw PlatformException(GetLastError());
	}

	MemoryPtr = mod;
	LoadedFromStream = false;
}

PIMAGE_DATA_DIRECTORY TPortableExecutableWin32::GetDataDirectories()
{
	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)MemoryPtr;

	PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((byte*)MemoryPtr + dos->e_lfanew);

	if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		PIMAGE_OPTIONAL_HEADER32 opt32 = (PIMAGE_OPTIONAL_HEADER32)(&nt->OptionalHeader);
		return opt32->DataDirectory;
	}
	else if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		PIMAGE_OPTIONAL_HEADER64 opt64 = (PIMAGE_OPTIONAL_HEADER64)(&nt->OptionalHeader);
		return opt64->DataDirectory;
	}

	return 0;
}

Array<String*>* TPortableExecutableWin32::GetExportedFunctionNames()
{
	Array<String*>* Result = new Array<String*>();

	PIMAGE_DATA_DIRECTORY directories = GetDataDirectories();

	PIMAGE_DATA_DIRECTORY exportDirPtr = &directories[ IMAGE_DIRECTORY_ENTRY_EXPORT ];

	PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)((byte*)MemoryPtr + exportDirPtr->VirtualAddress);

	int numOfFuncs = exportDir->NumberOfNames;
	ui32* funcNames = (ui32*)((byte*)MemoryPtr + exportDir->AddressOfNames);

	for (int i=0;i<numOfFuncs;i++)
	{
		ui32 curRva = funcNames[i];
		ch8* curPtr = (ch8*)((byte*)MemoryPtr+ curRva);

		

		String* funcName = new String(curPtr);
		Result->Add(funcName);
	}

	return Result;
}


#endif

