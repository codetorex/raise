#ifndef TTESTSUITEREPORTING_H
#define TTESTSUITEREPORTING_H

#include "ttest.h"
#include "tconvert.h"

/**
 * Not just tests, reports too.
 */
class TTestSuiteReport: public TTestSuite
{
private:
	char* curptr;

	/**
	 * This function can be overridden for getting text data to another place.
	 */
	virtual void AppendChar(char c)
	{
		*(curptr++) = c;
	}

	/**
	 * This function can be overridden for getting text data to another place.
	 */
	virtual void Append(const string& value)
	{
		MemoryDriver::Copy(curptr,value.Data,value.ByteLength);
		curptr += value.ByteLength;
	}

	/**
	 * This function can be overridden for getting text data to another place.
	 */
	virtual void Initialize()
	{
		Result = new char [1 * MB]; // BUFFER
		curptr = Result;
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
	char* Result;
	bool PrintOutputs;

	TTestSuiteReport(const string& _testName): TTestSuite(_testName)
	{
		PrintOutputs = false;
	}

	inline int Length()
	{
		return (int)(curptr - Result);
	}

	void RunSuite(ui32 tests = TEST_CHECK);
};

#endif