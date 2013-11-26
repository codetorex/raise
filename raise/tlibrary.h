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
	String		FunctionName;
	void*		FunctionPtr;
};

class TLibrary
{
private:
	void RegisterFunction(const String& funcName, void* funcPtr);

public:

	~TLibrary();

	Array< TLibraryFunction* > Functions;

	HMODULE		Library;
	String		Path;

	void		Load(const String& _path);
	void		Unload();
	void*		GetFunction(const String& functionName);
};

#endif