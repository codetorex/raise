#ifndef TTESTSUITEREPORTING_H
#define TTESTSUITEREPORTING_H

#include "ttest.h"
#include "tconvert.h"
#include "ttextwriter.h"

/**
 * Not just tests, reports too.
 */
class TTestSuiteReport: public TTestSuite
{
private:
	/**
	 * This function can be overridden for getting text data to another place.
	 */
	inline void AppendChar(char c)
	{
		Output->WriteChar(c);
	}

	/**
	 * This function can be overridden for getting text data to another place.
	 */
	inline void Append(const string& value)
	{
		Output->Write(value);
	}

	inline void AppendNewLine()
	{
		//AppendChar('\r'); // CR
		AppendChar('\n'); // LF
	}

	inline void AppendDashLine(int dashCount = 50)
	{
		for (int i=0;i<dashCount;i++)
		{
			AppendChar('-');
		}
		AppendNewLine();
	}

	inline void AppendPadded(const string& value,int pad= 8)
	{
		Append(value);
		for (int i = value.Length; i < pad ;i++)
		{
			AppendChar(' ');
		}
	}

	inline void AppendLine(const string& value)
	{
		Append(value);
		AppendNewLine();
	}

public:
	bool PrintOutputs;
	bool PrintApplicationInfo;
	bool PrintTestName;

	TTextWriter* Output;

	TTestSuiteReport(const string& _testName, TTextWriter* _output): TTestSuite(_testName), Output(_output)
	{
		PrintOutputs = false;
		PrintApplicationInfo = false;
		PrintTestName = true;
	}

	void RunSuite(ui32 tests = TEST_CHECK);
};

#endif