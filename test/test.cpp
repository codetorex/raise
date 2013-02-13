// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ttestsuitereporting.h"

#include "test_datetime.h"
#include "test_string.h"
#include "test_array.h"
#include "test_characterenumerator.h"
#include "test_network.h"
#include "test_queue.h"
#include "test_graphics.h"
#include "test_color.h"
#include "test_bitmap.h"
#include "test_xml.h"
#include "test_convert.h"
#include "test_matrix.h"
#include "test_mvc.h"

// TODO: test string builder
// there is an error in string builder, when you pass sfu(4245,-3) it crashes

#include "tapplication.h"
#include "raiseversion.h"
#include "tconsole.h"



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
// TODO: test stack
// TODO: test tstreamreader

// TODO: test rml writer
// TODO: test rml reader
// TODO: test string builder

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

#ifdef WIN32
int _tmain(int argc, wchar_t* argv[])
#else
int main(int argc,char** argv)
#endif
{
	Application.Begin( "RaiseLib Test Suite", TVersion(RAISE_BUILD, RAISE_PHASE), RAISE_DEFAULT_COMPANYNAME );
	USESRAISE;

	TBitmapFormats::CreateDefaultFormats();

	TTestSuiteReport suite("RaiseLib Test Suite", &Console);
	suite.PrintApplicationInfo = true;
	suite.PrintTestName = false;
	//suite.PrintOutputs = true;

	suite.AddTest(&TStringCheck);
	suite.AddTest(&TDateTimeCheck);
	suite.AddTest(&TArrayCheck);
	suite.AddTest(&TBitmapCheck);
	suite.AddTest(&TColorCheck);
	suite.AddTest(&TCharacterEnumeratorCheck);
	suite.AddTest(&NIPAddress4Check);
	suite.AddTest(&TQueueCheck);
	suite.AddTest(&TGraphicsBitmapCheck);
	suite.AddTest(&TXMLCheck);
	suite.AddTest(&TConvertCheck);
	suite.AddTest(&MMatrixCheck);
	suite.AddTest(&WMVCCheck);

	suite.RunSuite();
	
	getchar();


	return 0;
}

