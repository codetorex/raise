#ifndef CMD_EXPORTLIB_H
#define CMD_EXPORTLIB_H

#ifdef WIN32

#include "tcommandline.h"
#include "tapplet.h"
#include "ttestsuiteparameters.h"
#include "tpewin32.h"
#include "tstreamwriterutf8.h"

class TExportLibApplet: public TApplet
{
public:

	ui32 SimpleHash( const String& str )
	{
		unsigned char *p = str.Data;
		unsigned h = 2166136261;

		while (*p != 0)
		{
			h = ( h * 16777619 ) ^ *p;
			p++;
		}
			

		return h;
	}

	void Run(void* ParameterObj) 
	{
		TTestSuiteParameters* prm = (TTestSuiteParameters*)ParameterObj;

		TPortableExecutableWin32 libPe;

		Console.WriteLine("Opening library %...", sfs(prm->ExportLibraryName));

		libPe.LoadFromLibraryName(prm->ExportLibraryName);

		Console.WriteLine("Getting exported function names...");

		Array<String*>* funcNames = libPe.GetExportedFunctionNames();

		Console.WriteLine("Opening output file for write...");
		TFileStream* fs = new TFileStream(prm->OutputFile, fm_Write);
		TStreamWriterUTF8* sw = new TStreamWriterUTF8(fs);
		
		for (int i=0;i< funcNames->Count;i++ )
		{
			String* currentFunction = funcNames->Items[i];

			String line = String::Format("%_hash equ 0x%\r\n", sfs(*currentFunction), sfx(SimpleHash(*currentFunction)));
			sw->Write(line);

			Console.Insert(line);
		}

		Console.WriteLine("Exported % functions from % library successfully exported.", sfi(funcNames->Count), sfs(prm->ExportLibraryName));

		sw->Close();
	}
};



class CmdExportLib: public TCommandLineHandlerSimple
{
public:
	CmdExportLib()
	{
		Parameter = "--exportlib";
		ShortParameter = "-explib";
	}

	void ProcessParameter(TCommandLine* cmd)
	{
		TTestSuiteParameters* prm = (TTestSuiteParameters*)cmd->OptionObject;
		prm->Applet = new TExportLibApplet();
		prm->ExportLibraryName = cmd->GetNextParameter();
	}

} CmdExportLibHandler;

class CmdOutput: public TCommandLineHandlerSimple
{
public:
	CmdOutput()
	{
		Parameter = "--output";
		ShortParameter = "-out";
	}

	void ProcessParameter(TCommandLine* cmd)
	{
		TTestSuiteParameters* prm = (TTestSuiteParameters*)cmd->OptionObject;
		prm->OutputFile = cmd->GetNextParameter();
	}

} CmdOutputHandler;



#endif // WIN32

#endif