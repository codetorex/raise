#include "stdafx.h"

#ifdef WIN32

#include "tconsolewin32.h"



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

byte TConsoleWin32::ColorTransformTable[16] = {0,4,2,6,1,5,3,7,8,12,10,14,9,13,11,15};

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "tplatform.h"

void TConsoleWin32::CreateConsole( const String& title )
{
	AllocConsole();

	if (!(title == String::Empty))
	{
		ch16 wtitle[512];
		Platform.RaiseToSystemString(title,(byte*)wtitle,sizeof(wtitle));
		SetConsoleTitleW(wtitle);
	}
	//

	int hConHandle;
	long lStdHandle;
	FILE *fp;

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
}

#endif