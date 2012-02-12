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
#include "tutf16encoding.h"
#include "tcommandline.h"

class MGenParameters
{
public:
	bool InputFileSpecified;
	bool OutputFileSpecified;
} Params;

class MVersionHandler: public TCommandLineHandlerSimple
{
public:
	MVersionHandler()
	{
		Parameter = "--version";
		ShortParameter = "-v";
	}

	void ProcessParameter(TCommandLine* cmd)
	{
		Application.PrintIdentifierToConsole();
	}

} VersionHandler;

class MHelpHandler: public TCommandLineHandlerSimple
{
public:
	MHelpHandler()
	{
		Parameter = "--help";
		ShortParameter = "-h";
	}

	void ProcessParameter(TCommandLine* cmd)
	{
		Console.WriteLine("Help will be shown");
	}

} HelpHandler;

int _tmain(int argc, _TCHAR* argv[])
{
	Application.Begin( "MGen", TVersion(MGEN_BUILD,MGEN_PHASE) );

	TCommandLine CmdLine(&Params);

	

	try
	{
		CmdLine.RegisterHandler(&VersionHandler);
		CmdLine.RegisterHandler(&HelpHandler);

		CmdLine.Initialize(argc,argv);
		CmdLine.ParseCommandLine();
	}
	catch(Exception& e)
	{
		Console.WriteLine("Error occurred while parsing command line parameters:");
		Console.WriteLine(e.Message);

	}

	

	if (!Params.InputFileSpecified || !Params.OutputFileSpecified)
	{
		Console.WriteLine("Missing parameters");
		Console.Write("type 'mgen -h' for help");
	}


	Console.ReadKey();

	return 0;
}

