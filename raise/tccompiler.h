#ifndef TCCOMPILER_H
#define TCCOMPILER_H

#include "tstring.h"

struct TCCState;

/**
 * Tiny C Compiler Wrapper
 */
class TCCompiler
{
public:
	void AddIncludePath(const String& path);
	void AddSystemIncludePath(const String& path);
	void DefineSymbol(const String& symbol, const String& value);
	void UndefineSymbol(const String& symbol);

	void AddFile(const String& path);
	
	void CompileString(const String& code);



private:
	TCCState* State;
};


#endif