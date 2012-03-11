#include "stdafx.h"
#include "tconsole.h"

#ifdef WIN32


	/*TConsoleWindows()
	{
		ColorTransformTable[CC_BLACK]   = 0;
		ColorTransformTable[CC_MAROON]  = 4;
		ColorTransformTable[CC_GREEN]   = 2;
		ColorTransformTable[CC_OLIVE]   = 6;
		ColorTransformTable[CC_NAVY]    = 1;
		ColorTransformTable[CC_PURPLE]  = 5;
		ColorTransformTable[CC_TEAL]    = 3;
		ColorTransformTable[CC_SILVER]  = 7;
		ColorTransformTable[CC_GRAY]    = 8;
		ColorTransformTable[CC_RED]     = 12;
		ColorTransformTable[CC_LIME]    = 10;
		ColorTransformTable[CC_YELLOW]  = 14;
		ColorTransformTable[CC_BLUE]    = 9;
		ColorTransformTable[CC_FUCHSIA] = 13;
		ColorTransformTable[CC_AQUA]    = 11;
		ColorTransformTable[CC_WHITE]   = 15;
	}*/

byte TConsoleWindows::ColorTransformTable[16] = {0,4,2,6,1,5,3,7,8,12,10,14,9,13,11,15};

TConsoleWindows Console;

#endif