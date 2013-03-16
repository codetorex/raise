#ifndef TCONSOLE_H
#define TCONSOLE_H


#ifdef WIN32
#include "tconsolewin32.h"
#else
#include "tconsolevt100.h"
typedef TConsoleVT100 TConsoleCurrent;
#endif

extern TConsoleCurrent Console;

#endif