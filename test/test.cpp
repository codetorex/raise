// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "tconsolereport.h"

#include "test_datetime.h"
#include "test_string.h"
#include "test_array.h"

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



#ifdef WIN32
int _tmain(int argc, wchar_t* argv[])
#else
int main(int argc,char** argv)
#endif
{
	TConsoleReport suite("RaiseLib Test Suite");

	suite.AddTest(&TStringCheck);
	suite.AddTest(&TDateTimeCheck);
	suite.AddTest(&TArrayCheck);
	
	//suite.PrintOutputs = true;
	suite.RunSuite();

	getchar();

	return 0;
}

