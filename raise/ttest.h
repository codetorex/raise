#ifndef TTEST_H
#define TTEST_H

// Unit testing helper classes


#include "tprofiler.h"
#include "texception.h"
#include "tarray.h"
#include "tstring.h"

// char* used as test names because maybe we test the string class


enum TestTypes
{
	TEST_BASE = 0,
	TEST_CHECK = 1,
	TEST_PERFORMANCE = 2,
	TEST_COMPARISON = 4,
	TEST_SUITE = 8,
};

enum TestResults
{
	TEST_FAILED,
	TEST_PASS,
	TEST_OUTPUT,
};

class TTestResult
{
public:
	TestResults Result;
	string ResultComment;
};

/**
 * Class testing class.
 */
class TTest
{
public:
	string TestName;
	ui32 TestType;


	TTest(const string& _testName)
	{
		TestName = _testName;
		TestType = TEST_BASE;
	}

	virtual void Test() = 0;
};

class TTestCheck: public TTest
{
public:
	TArray< TTestResult* > Results;

	int CountPass;
	int CountFail;
	int CountTotal;
	int CountOutput;
	
	TTestCheck(const string& _testName): TTest(_testName) 
	{
		TestType = TEST_CHECK;
		CountPass = 0;
		CountFail = 0;
		CountTotal = 0;
		CountOutput = 0;
	}

	void AddResult(bool passed, const string& comment);
	void AddOutput(const string& comment);

	virtual void Test() = 0;
};

class TTestPerformance: public TTest
{
public:
	ui32 LoopCount;
	TProfiler Profiler;

	TTestPerformance(const string& _testName): TTest(_testName) {}

	TTestPerformance(const string& _testName, ui32 _loopCount):TTest(_testName)
	{
		LoopCount = _loopCount;
		TestType = TEST_PERFORMANCE;
	}

	void Test();

	virtual void PerformanceTest() = 0;
};

class TTestComparison: public TTest
{
public:

	TTestComparison(const char* _testName):TTest(_testName) 
	{
		TestType = TEST_COMPARISON;
	}

	/// Algorithms performances to compare
	TArray< TTestPerformance* > Algorithms;

	void Test()
	{
		int algCount = Algorithms.Count;

		while(algCount--)
		{
			Algorithms.Item[algCount]->Test();
		}
	}
};

class TTestSuite: public TTest
{
public:
	TTestSuite(const string& _testName): TTest(_testName)
	{
		TestType = TEST_SUITE;
	}

	TArray< TTest* > Tests;

	inline void AddTest(TTest* test)
	{
		Tests.Add(test);
	}
	
	/**
	 * @brief Loops through tests and does test.
	 * @param tests Which test types going to be tested.
	 */
	virtual void RunSuite(ui32 tests = TEST_CHECK);

	void Test()
	{
		RunSuite();
	}
};



#endif