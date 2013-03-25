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
		suite.AddTest(&TRaiseMarkdownCheck);
		suite.AddTest(&WMVCCheck);
		suite.AddTest(&WFalconRendererCheck);

		suite.RunSuite();

		Console.ReadKey();
	}

};

#endif