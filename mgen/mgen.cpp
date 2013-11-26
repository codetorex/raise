/*

Member info generating preprocessor.

*/
#include "stdafx.h"

#include "tstreamreaderutf8.h"
#include "tlog.h"
#include "tapplication.h"
#include "mgenversion.h"
#include "mcmdlineparsers.h"
#include "tpath.h"
#include "tstreamreader.h"

#include "mgensourceparser.h"

class MGenDatabase
{
public:
	Array< MGenSource* > Sources;

	void LoadCPPSource(const String& sourcePath)
	{
		MGenSource* msrc = new MGenSource();
		
		TStreamReader* src;
		src = new TStreamReader(sourcePath);

		int lineno = 0;
		while(!src->EndOfStream)
		{
			String currentLine = src->ReadLine();
			MGenSourceLine* mline = new MGenSourceLine();
			mline->LineNumber = lineno;
			mline->Line = currentLine;
			msrc->Lines.Add(mline);
			lineno++;
		}

		Sources.Add(msrc);
		src->Close();

		MGenSourceParser msp(msrc);
		msp.Parse();
	}

	void SaveDatabase(const String& targetPath);
	void LoadDatabase(const String& targetPath);
} Database;

int _tmain(int argc, _TCHAR* argv[])
{
	Application.Begin( "MGen", TVersion(MGEN_BUILD,MGEN_PHASE) );

	TCommandLine CmdLine(&Params);

	try
	{
		CmdLine.RegisterHandler(&ExePathHandler);
		CmdLine.RegisterHandler(&VersionHandler);
		CmdLine.RegisterHandler(&HelpHandler);

		CmdLine.RegisterHandler(&InputHandler);
		CmdLine.RegisterHandler(&DatabaseHandler);

		CmdLine.Initialize(argc,argv);
		CmdLine.ParseCommandLine();
	}
	catch(Exception& e)
	{
		Console.WriteLine("Error occurred while parsing command line parameters:");
		Console.WriteLine(e.Message);

	}

	if ((!Params.InputFileSpecified || !Params.DatabaseFileSpecified))
	{
		if (!Params.InfoCommand)
		{
			Console.WriteLine("Missing parameters");
			Console.Write("type 'mgen -h' for help");
		}
		
		return 0;
	}



	String ext = TPath::GetExtension( Params.InputFile );

	if (ext == ".h" || ext == ".cpp")
	{
		try
		{
			Database.LoadCPPSource(Params.InputFile);
		}
		catch(Exception& e)
		{
			Console.WriteLine("Error occurred while parsing source file (%): %", sfs(Params.InputFile),sfs(e.Message));
		}
	}


	

	// PROCESS FILES HERE

	Console.ReadKey();

	//Console.ReadKey();

	return 0;
}

