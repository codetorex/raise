// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "cmd_test.h"
#include "cmd_exportlib.h"




#include "tapplication.h"
#include "raiseversion.h"
#include "tcommandline.h"
#include "ttestsuiteparameters.h"






/*#include "trmlwriter.h"
#include "tfile.h"

#include "tlogstream.h"
#include "nasynchronouswindows.h"
#include "nservicehttpbasic.h"

class TLogConsole: public TLogOutput
{
public:
	void Output( TLogEntry* entry ) 
	{
		byte stackbuffer[512];

		TStringBuilder sb(stackbuffer,512);



		sb.Append(Log.Groups.Item[entry->GroupID]->ShortName);
		sb.AppendChar('|');
		sb.AppendChar(' ');


		sb.Append(sfx(TThread::get_CurrentThreadID(),-6));
		sb.AppendChar('|');
		sb.AppendChar(' ');


		sb.AppendPadded(entry->Tick,-8,' ');
		sb.AppendChar('|');
		sb.AppendChar(' ');


		if (entry->Content.Length > 386)
		{
			sb.Append("!!! Entry too long !!!");
		}
		else
		{
			sb.Append(entry->Content);
		}
		
		sb.AppendLine();

		sb.PokeZero();

		printf((char*)sb.GetData());
		

		sb.UnbindByteArray();
	}
};*/

#include <iostream>       // std::cerr
#include <exception>      // std::set_terminate
#include <cstdlib>        // std::abort

#ifdef LINUX

#include <stdexcept>

#include <execinfo.h>

void myterminate () {
	void *trace_elems[20];
	int trace_elem_count(backtrace( trace_elems, 20 ));
	char **stack_syms(backtrace_symbols( trace_elems, trace_elem_count ));
	for ( int i = 0 ; i < trace_elem_count ; ++i )
	{
		std::cout << stack_syms[i] << "\n";
	}
	free( stack_syms );

	exit(1);
}

#endif // LINUX

#ifdef WIN32
int _tmain(int argc, wchar_t* argv[])
#else
int main(int argc,char** argv)
#endif
{
#ifdef LINUX
	std::set_terminate (myterminate);
#endif // LINUX

	Application.Begin( "RaiseLib Test Suite", TVersion(RAISE_BUILD, RAISE_PHASE), RAISE_DEFAULT_COMPANYNAME );
	USESRAISE;

	Console.Initialize();

	TTestSuiteParameters Params;
	Params.Applet = new TTestApplet();

	TCommandLine CmdLine(&Params);

	try
	{
#ifdef WIN32
		CmdLine.RegisterHandler(&CmdExePathHandler);
		CmdLine.RegisterHandler(&CmdExportLibHandler);
		CmdLine.RegisterHandler(&CmdOutputHandler);
#endif // WIN32

		CmdLine.Initialize(argc,argv);
		CmdLine.ParseCommandLine();
	}
	catch(Exception& e)
	{
		Console.WriteLine("Error occurred while parsing command line parameters:");
		Console.WriteLine(e.Message);
	}

	Params.Applet->Run(&Params);
	
	Console.ReadKey();
	return 0;
}

