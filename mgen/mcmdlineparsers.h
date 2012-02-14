#ifndef MCMDLINEPARSERS_H
#define MCMDLINEPARSERS_H

#include "tconsole.h"
#include "tencoding.h"
#include "tutf16encoding.h"
#include "tcommandline.h"

class MGenParameters
{
public:

	TString ExePath;
	bool	InfoCommand;
	bool	InputFileSpecified;
	bool	DatabaseFileSpecified;

	TString	InputFile;
	TString DatabaseFile; // this file will hold the all the classes that parsed, so we can import them anywhere we want

	MGenParameters()
	{
		InfoCommand = false;
	}
} Params;

class MExePathHandler: public TCommandLineHandler
{
public:

	HandleResult HandleParameter( TCommandLine* cmd ) 
	{
		if (cmd->ParameterIndex == 0)
		{
			MGenParameters* prm = (MGenParameters*)cmd->OptionObject;
			prm->ExePath = cmd->GetCurrentParameter();
			return TCommandLineHandler::HR_Handled;
		}

		return TCommandLineHandler::HR_Continue;
	}
} ExePathHandler;

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
		MGenParameters* prm = (MGenParameters*)cmd->OptionObject;
		prm->InfoCommand = true;
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
		MGenParameters* prm = (MGenParameters*)cmd->OptionObject;
		prm->InfoCommand = true;
		Console.WriteLine("Help will be shown");
	}

} HelpHandler;

class MInputHandler: public TCommandLineHandlerSimple
{
public:
	MInputHandler()
	{
		Parameter = "--input";
		ShortParameter = "-i";
	}

	void ProcessParameter(TCommandLine* cmd)
	{
		MGenParameters* prm = (MGenParameters*)cmd->OptionObject;
		prm->InputFile = cmd->GetNextParameter();
		prm->InputFileSpecified = true;
		Console.WriteLine("Source file: %", sfs(prm->InputFile));
	}

} InputHandler;

class MDatabaseHandler: public TCommandLineHandlerSimple
{
public:
	MDatabaseHandler()
	{
		Parameter = "--database";
		ShortParameter = "-d";
	}

	void ProcessParameter(TCommandLine* cmd)
	{
		MGenParameters* prm = (MGenParameters*)cmd->OptionObject;
		prm->DatabaseFile = cmd->GetNextParameter();
		prm->DatabaseFileSpecified = true;
		Console.WriteLine("Database file: %", sfs(prm->DatabaseFile));
	}

} DatabaseHandler;


#endif