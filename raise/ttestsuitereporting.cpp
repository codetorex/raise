#include "stdafx.h"
#include "ttestsuitereporting.h"
#include "texception.h"
#include "tapplication.h"


void TTestSuiteReport::RunSuite( ui32 tests /*= TEST_CHECK*/ )
{
	AppendDashLine();
	if (PrintTestName)
	{
		AppendLine(TestName);
	}

	if (PrintApplicationInfo)
	{
		AppendLine(Application.IdentifyText);
	}

	AppendDashLine();

	for (ui32 i=0;i<Tests.Count;i++)
	{
		TTest* curTest = Tests[i];
		if ( !(curTest->TestType & tests))
			continue;


		Append("    + ");
		AppendPadded(curTest->TestName,30);

		curTest->Execute();

		if (curTest->TestType == TEST_CHECK)
		{
			UnitTest* curCheck = (UnitTest*)curTest;
			Append("Pass:");
			AppendPadded( Convert::ToString(curCheck->CountPass) , 5 );
			Append("Fail:");
			AppendPadded( Convert::ToString(curCheck->CountFail) , 5 );
			Append("Total:");
			AppendPadded( Convert::ToString(curCheck->CountTotal) , 5 );

			if (curCheck->CountFail > 0 || ( curCheck->CountOutput > 0 && PrintOutputs ) )
			{
				AppendNewLine();
				for (ui32 k =0;k<curCheck->Results.Count;k++)
				{
					TTestResult* curResult = curCheck->Results[k];
					if (curResult->Result == TEST_FAILED)
					{
						Append("        - ");
						AppendLine(curResult->ResultComment);
					}
					else if (curResult->Result == TEST_OUTPUT && PrintOutputs)
					{
						Append("        * ");
						AppendLine(curResult->ResultComment);
					}
				}
			}
		}
		else if (curTest->TestType == TEST_PERFORMANCE)
		{
			throw NotImplementedException(__FILE__,__LINE__);
		}
		else if (curTest->TestType == TEST_COMPARISON)
		{
			throw NotImplementedException(__FILE__,__LINE__);
		}
		AppendNewLine();
	}

	AppendDashLine();
}