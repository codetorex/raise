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
	void AddIncludePath(const TString& path);
	void AddSystemIncludePath(const TString& path);
	void DefineSymbol(const TString& symbol, const TString& value);
	void UndefineSymbol(const TString& symbol);

	void AddFile(const TString& path);
	
	void CompileString(const TString& code);



private:
	TCCState* State;
};


#endif