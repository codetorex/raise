#ifndef TPEWIN32_H
#define TPEWIN32_H

#ifdef WIN32

#include "tstream.h"
#include <windows.h>

class TPortableExecutableWin32
{
public:
	void* MemoryPtr; // maybe define this as stream? so we can read other processes too?
	bool LoadedFromStream;


	TPortableExecutableWin32()
	{
		MemoryPtr = 0;
		LoadedFromStream = false;
	}

	~TPortableExecutableWin32()
	{
		if (LoadedFromStream)
		{
			delete MemoryPtr;
		}
	}


	/**
	 * Loads stream until it ends to MemoryPtr
	 */
	void LoadFromStream(TStream* stream);


	void LoadFromLibraryName(const TString& libraryName);

	PIMAGE_DATA_DIRECTORY GetDataDirectories();

	TArray<string*>* GetExportedFunctionNames();
};

#endif // WIN32


#endif // !TPEWIN32_H
