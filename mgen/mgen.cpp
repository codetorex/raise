/*

Member info generating preprocessor.

*/
#include "stdafx.h"

#include "tstreamreaderutf8.h"
#include "tlog.h"
#include "tapplication.h"
#include "mgenversion.h"
#include "tconsole.h"
#include "tencoding.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Application.Begin( "MGen", TVersion(MGEN_BUILD,MGEN_PHASE) );

	if (argc < 2)
	{
		Console.WriteLine("Missing parameters");
		Console.Write("type 'mgen -h' for help");
	}

	if (argc == 2)
	{
		TString arg1 = TEncoding::UTF16.GetString(argv[1]);
		if (arg1 == "--version" || arg1 == "-v")
		{
			Application.PrintIdentifierToConsole();
			return 0;
		}
	}


	return 0;
}

