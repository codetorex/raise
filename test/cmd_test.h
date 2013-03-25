#ifndef CMD_TEST_H
#define CMD_TEST_H

#include "tapplet.h"

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
#include "test_markdown.h"
#include "test_falconrenderer.h"
#include "test_hawkrenderer.h"
#include "test_stringreader.h"

#ifdef WIN32
//#include <strsafe.h>
#endif // WIN32



#include "tconsole.h"


// TODO: test string builder
// there is an error in string builder, when you pass sfu(4245,-3) it crashes

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

class TTestApplet: public TApplet
{
public:
	void Run(void* ParameterObj) 
	{
		TBitmapFormats::CreateDefaultFormats();

		TTestSuiteReport suite("RaiseLib Test Suite", &Console);
		suite.PrintApplicationInfo = true;
		suite.PrintTestName = true;
		suite.PrintOutputs = true;

		
		suite.AddTest(&TArrayCheck);
		suite.AddTest(&TStringCheck);
		suite.AddTest(&TStringReaderCheck);
		suite.AddTest(&TCharacterEnumeratorCheck);
		suite.AddTest(&TDateTimeCheck);
		suite.AddTest(&TConvertCheck);
		suite.AddTest(&TQueueCheck);
		suite.AddTest(&TColorCheck);		
		suite.AddTest(&TBitmapCheck);
		suite.AddTest(&TGraphicsBitmapCheck);
		suite.AddTest(&NIPAddress4Check);
		suite.AddTest(&TXMLCheck);
		suite.AddTest(&MMatrixCheck);
		suite.AddTest(&TRaiseMarkdownCheck);
		suite.AddTest(&WMVCCheck);
		//suite.AddTest(&WFalconRendererCheck);
		suite.AddTest(&WHawkRendererCheck);

		// TODO: implement test grouping so we can have categorized test results.

		suite.RunSuite();

		Console.ReadKey();
	}

};

#endif