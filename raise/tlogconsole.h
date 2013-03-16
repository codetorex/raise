#ifndef TLOGCONSOLE_H
#define TLOGCONSOLE_H

#include "tlog.h"
#include "tconsole.h"

class TLogConsole: public TLogOutput
{
public:
	TConsole* ConsoleOutput;
	

	TLogConsole(TConsole* _ConsoleOutput)
	{
		ConsoleOutput = _ConsoleOutput;
	}

	void Output(TLogEntry* entry);
};


#endif