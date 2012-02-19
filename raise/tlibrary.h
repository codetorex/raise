#ifndef TLIBRARY_H
#define TLIBRARY_H

#include "tstring.h"

#ifdef LINUX
// TODO: merge shit like these in a single header file and include it in pre-compiled header
typedef void*	HMODULE;
#endif

class TLibraryFunction
{
public:
	TString		FunctionName;
	void*		FunctionPtr;
};

class TLibrary
{
private:
	void RegisterFunction(const TString& funcName, void* funcPtr);

public:

	~TLibrary();

	TArray< TLibraryFunction* > Functions;

	HMODULE		Library;
	TString		Path;

	void		Load(const TString& _path);
	void		Unload();
	void*		GetFunction(const TString& functionName);
};

#endif