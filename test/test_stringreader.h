#ifndef TEST_STRINGREADER_H
#define TEST_STRINGREADER_H

#include "ttest.h"
#include "tstringreader.h"



class TestTStringReader: public UnitTest
{
public:

	TestTStringReader(): UnitTest("TStringReader Check") 
	{

	}

	void Execute()
	{
		Should("Let you peek a char");
		{
			String testString = "Test String";
			TStringReader sr(testString);
			ch32 result = sr.Peek();
			Result(result == 'T' && sr.CurrentIndex == 0 && sr.CurrentByteIndex == 0); // its not advanced the index
		}

		Should("Read a character");
		{
			String testString = "Test String";
			TStringReader sr(testString);
			ch32 result = sr.Read();
			Result(result == 'T' && sr.CurrentIndex == 1 && sr.CurrentByteIndex == 1); // its not advanced the index

			Should("Read second character");
			result = sr.Read();
			Result(result == 'e' && sr.CurrentIndex == 2 && sr.CurrentByteIndex == 2);

			Should("Read third character");
			result = sr.Read();
			Result(result == 's' && sr.CurrentIndex == 3 && sr.CurrentByteIndex == 3);
		}

		Should("Read a line");
		{
			String testString = "First line\nSecond Line\nThird Line";
			TStringReader sr(testString);
			String firstLine = sr.ReadLine();
			Result(firstLine == "First line");

			Should("Read second line");
			String secondLine = sr.ReadLine();
			Result(secondLine == "Second Line");

			Should("Read third line");
			String thirdLine = sr.ReadLine();
			Result(thirdLine == "Third Line");
		}

		Should("Read until encountering some character");
		{
			String testString = "Test string @Data.Load(\"test\")";
			TStringReader sr(testString);
			int interrupt;
			String firstPart = sr.ReadUntil("@", String::Empty,interrupt);
			Result(firstPart == "Test string " && interrupt == '@');

			Should("Read second part");
			String secondPart = sr.ReadUntil(" ", String::Empty, interrupt);
			Result(secondPart == "Data.Load(\"test\")" && interrupt == -1);
		}

		Should("Read until matching a given string");
		{
			String testString = "Test string @Data.Load(\"test\")";
			TStringReader sr(testString);
			String firstPart = sr.ReadUntilString("@Data");
			Result(firstPart == "Test string ");



		}

		Should("Omit search pattern from original text");
		{
			TStringReader sr("Test text <$ Test Code $> End Text");
			String textPart = sr.ReadUntilString("<$");
			String codePart = sr.ReadUntilString("$>");

		
			Result(textPart == "Test text ");

			Should("Not read more than pattern itself");
			Result(codePart == " Test Code ");
		}

	}

} TStringReaderCheck;

#endif