#ifndef TLOGCONSOLE_H
#define TLOGCONSOLE_H

#include "tlog.h"
#include "tconsoledriver.h"

class TLogConsole: public TLogOutput
{
public:
	TConsoleDriver* ConsoleOutput;
	

	TLogConsole(TConsoleDriver* _ConsoleOutput)
	{
		ConsoleOutput = _ConsoleOutput;
	}

	void Output(TLogEntry* entry);
};


#endif