#include "stdafx.h"
#include "ttest.h"

void UnitTest::Result( bool passed, const String& comment )
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

void UnitTest::Output( const String& comment )
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

	for (ui32 i=0;i< LoopCount;i+=5)
	{
		PerformanceTest();
		PerformanceTest();
		PerformanceTest();
		PerformanceTest();
		PerformanceTest();
	}

	Profiler.EndProfiling();
}

void TTestSuite::RunSuite( ui32 tests /*= TEST_CHECK*/ )
{
	for (ui32 i=0;i<Tests.Count;i++)
	{
		TTest* curTest = Tests.Items[i];
		if ( (curTest->TestType & tests))
		{
			curTest->Execute();
		}
	}
}