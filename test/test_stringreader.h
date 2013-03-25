#ifndef TEST_STRINGREADER_H
#define TEST_STRINGREADER_H

#include "ttest.h"
#include "tstringreader.h"



class TestTStringReader: public TTestCheck
{
public:

	TestTStringReader(): TTestCheck("TStringReader Check") 
	{

	}

	void Test()
	{
		Should("Let you peek a char");
		{
			TString testString = "Test String";
			TStringReader sr(testString);
			ch32 result = sr.Peek();
			AddResult(result == 'T' && sr.CurrentIndex == 0 && sr.CurrentByteIndex == 0); // its not advanced the index
		}

		Should("Read a character");
		{
			TString testString = "Test String";
			TStringReader sr(testString);
			ch32 result = sr.Read();
			AddResult(result == 'T' && sr.CurrentIndex == 1 && sr.CurrentByteIndex == 1); // its not advanced the index

			Should("Read second character");
			result = sr.Read();
			AddResult(result == 'e' && sr.CurrentIndex == 2 && sr.CurrentByteIndex == 2);

			Should("Read third character");
			result = sr.Read();
			AddResult(result == 's' && sr.CurrentIndex == 3 && sr.CurrentByteIndex == 3);
		}

		Should("Read a line");
		{
			TString testString = "First line\nSecond Line\nThird Line";
			TStringReader sr(testString);
			TString firstLine = sr.ReadLine();
			AddResult(firstLine == "First line");

			Should("Read second line");
			TString secondLine = sr.ReadLine();
			AddResult(secondLine == "Second Line");

			Should("Read third line");
			TString thirdLine = sr.ReadLine();
			AddResult(thirdLine == "Third Line");
		}

		Should("Read until encountering some character");
		{
			TString testString = "Test string @Data.Load(\"test\")";
			TStringReader sr(testString);
			int interrupt;
			TString firstPart = sr.ReadUntil("@", TString::Empty,interrupt);
			AddResult(firstPart == "Test string " && interrupt == '@');

			Should("Read second part");
			TString secondPart = sr.ReadUntil(" ", TString::Empty, interrupt);
			AddResult(secondPart == "Data.Load(\"test\")" && interrupt == -1);
		}

	}

} TStringReaderCheck;

#endif