#include "stdafx.h"
#include "tlibrary.h"
#include "texception.h"

TLibrary::~TLibrary()
{
	Functions.DeletePointers();
}

void TLibrary::RegisterFunction(const TString& funcName, void* funcPtr)
{
	TLibraryFunction* fnc = new TLibraryFunction();
	fnc->FunctionName = funcName;
	fnc->FunctionPtr = funcPtr;
	Functions.Add(fnc);
}

#ifdef WIN32

#include "twintools.h"

void TLibrary::Load(const TString& _path)
{
	ch16 tmp[1024];
	TWinTools::SystemString16(_path,tmp,1024);

	Path = _path;
	Library = LoadLibrary(tmp);

	if (Library == NULL)
	{
		ui32 err = GetLastError();
		throw Exception("Error occurred while loading library [%]: %", sfs(Path),sfs(TWinTools::ErrorToStringWithCode(err)));
	}
}

void TLibrary::Unload()
{
	if ( FreeLibrary(Library) == 0 )
	{
		ui32 err = GetLastError();
		throw Exception("Error occurred while unloading library [%]: %", sfs(Path),sfs(TWinTools::ErrorToStringWithCode(err)));
	}
}

void* TLibrary::GetFunction( const TString& functionName )
{
	if (!functionName.IsASCII())
	{
		throw Exception("Function name should be ascii compliant");
	}

	void* funcPtr = GetProcAddress(Library,(char*)functionName.Data);

	if (funcPtr == 0)
	{
		ui32 err = GetLastError();
		throw Exception("Error occurred while loading function in library [%] Function: % Error: %", sfs(Path),sfs(functionName),sfs(TWinTools::ErrorToStringWithCode(err)));
	}

	RegisterFunction(functionName, funcPtr);
	return funcPtr;
}

#else

#include <dlfcn.h>

/*

void *dlopen(const char *filename, int flag);

char *dlerror(void);

void *dlsym(void *handle, const char *symbol);

int dlclose(void *handle);

*/


#endif