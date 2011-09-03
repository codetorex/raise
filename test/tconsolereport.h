#ifndef TCONSOLEREPORT_H
#define TCONSOLEREPORT_H

#include "ttestsuitereporting.h"

class TConsoleReport: public TTestSuiteReport
{
public:
	TConsoleReport(const string& _testName): TTestSuiteReport(_testName)
	{

	}

private:
	void AppendChar(char c)
	{
		putchar(c);
	}

	void Append(const string& value)
	{
		char* src = (char*)value.Data;
		char d;
		while( d = *(src++) )
		{
			putchar(d);
		}
	}

	void Initialize()
	{

	}
};


#endif


