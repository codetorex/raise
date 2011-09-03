#include "stdafx.h"
#include "ttest.h"

void TTestCheck::AddResult( bool passed, const string& comment )
{
	TTestResult* tr = new TTestResult();
	tr->Result = passed ? TEST_PASS : TEST_FAILED;
	tr->ResultComment = comment;
	Results.Add(tr);

	CountTotal++;
	if (passed)
	{
		CountPass++;
	}
	else
	{
		CountFail++;
	}
}

void TTestCheck::AddOutput( const string& comment )
{
	TTestResult* tr = new TTestResult();
	tr->Result = TEST_OUTPUT;
	tr->ResultComment = comment;
	Results.Add(tr);
	CountOutput++;
}

void TTestPerformance::Test()
{
	Profiler.BeginProfiling(LoopCount);

	for (dword i=0;i< LoopCount;i+=5)
	{
		PerformanceTest();
		PerformanceTest();
		PerformanceTest();
		PerformanceTest();
		PerformanceTest();
	}

	Profiler.EndProfiling();
}

void TTestSuite::RunSuite( dword tests /*= TEST_CHECK*/ )
{
	for (dword i=0;i<Tests.Count;i++)
	{
		TTest* curTest = Tests.Item[i];
		if ( (curTest->TestType & tests))
		{
			curTest->Test();
		}
	}
}