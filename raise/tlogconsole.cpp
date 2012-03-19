#include "stdafx.h"
#include "tlogconsole.h"


void TLogConsole::Output( TLogEntry* entry )
{
	switch (entry->GroupID)
	{
	case LG_INF:
		ConsoleOutput->SetForeColor(CC_WHITE);
		break;

	case LG_ERR:
	case LG_FTL:
		ConsoleOutput->SetForeColor(CC_RED);
		break;

	case LG_WRN:
		ConsoleOutput->SetForeColor(CC_YELLOW);
		break;

	case LG_DBG:
	case LG_TRC:
		ConsoleOutput->SetForeColor(CC_AQUA);
		break;
	}

	ConsoleOutput->WriteLine(entry->Content);
}