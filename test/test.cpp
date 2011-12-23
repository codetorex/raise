// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "tconsolereport.h"

#include "test_datetime.h"
#include "test_string.h"
#include "test_array.h"


#include "tcolor.h"
#include "tapplication.h"
#include "raiseversion.h"

class TestTColor: public TTestCheck
{
public:
	TestTColor(): TTestCheck("TColor check") {};

	void Test()
	{
		TColor32 clr(64,128,178,255);

		TStringBuilder sb;
		sb.Append("R: ");
		sb.AppendPadded(clr.r,3,' ');
		sb.Append("G: ");
		sb.AppendPadded(clr.g,3,' ');
		sb.Append("B: ");
		sb.AppendPadded(clr.b,3,' ');
		sb.Append("A: ");
		sb.AppendPadded(clr.a,3,' ');

		sb.Append("clr[0] = ");
		sb.AppendPadded(clr.bclr[0],3,' ');
		sb.Append("clr[1] = ");
		sb.AppendPadded(clr.bclr[1],3,' ');
		sb.Append("clr[2] = ");
		sb.AppendPadded(clr.bclr[2],3,' ');
		sb.Append("clr[3] = ");
		sb.AppendPadded(clr.bclr[3],3,' ');

		//AddOutput(sb.ToString());

		//AddResult( clr.r == 255 && clr.g == 0 && clr.b == 255 && clr.a == 255, "Color r,g,b constructor" );
		
/*		TColor32ARGB argb = clr;
		
		AddResult( argb.r == 255 && argb.g == 0 && argb.b == 255 && argb.a == 255, "Color r,g,b constructor" );*/
	
		//TStringBuilder sb;
		
	}
} TColorCheck;

// TODO: test utf 16 encoding class
// TODO: test buffered stream
// TODO: test composite buffer shit
// TODO: test bitmap classes
// TODO: test hash map
// TODO: test color classes
// TODO: test bitset, bitfield, bitstack, binary
// TODO: test dictionary class
// TODO: implement and test index class
// TODO: test linked list
// TODO: test queue
// TODO: test stack
// TODO: test tstreamreader

// TODO: test rml writer
// TODO: test rml reader
// TODO: test string builder

#include "trmlwriter.h"
#include "tfile.h"

#include "tlogtext.h"
#include "nserverwindows.h"


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

		sb.Append(entry->Content);
		sb.AppendLine();

		sb.PokeZero();

		printf((char*)sb.GetData());
		

		sb.UnbindByteArray();
	}
};

#ifdef WIN32
int _tmain(int argc, wchar_t* argv[])
#else
int main(int argc,char** argv)
#endif
{
	Application.Begin( "RaiseLib Test Suite", RaiseModule.Version );

	/*TConsoleReport suite("RaiseLib Test Suite");
	suite.PrintApplicationInfo = true;
	suite.PrintTestName = false;

	suite.AddTest(&TStringCheck);
	suite.AddTest(&TDateTimeCheck);
	suite.AddTest(&TArrayCheck);
	suite.AddTest(&TColorCheck);

	//suite.PrintOutputs = true;
	suite.RunSuite();*/
	
	TLogText textLog(new TFileStream("log.txt", fm_Write));
	TLogConsole console;
	Log.RegisterOutput(&textLog);
	Log.RegisterOutput(&console);

	NServerWindows winSrv;

	NServiceHTTP http;

	http.RootFolder = "F:\\Inetpub\\wwwroot\\11FenC";

	//winSrv.Initialize();

	winSrv.AddService(&http);
	winSrv.CreateListener(NIPAddress("127.0.0.1"),31,NP_TCP,&http);


	winSrv.MainThreadFunction();

	getchar();


	return 0;
}

