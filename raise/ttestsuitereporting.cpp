#include "stdafx.h"
#include "ttestsuitereporting.h"
#include "texception.h"


void TTestSuiteReport::RunSuite( dword tests /*= TEST_CHECK*/ )
{
	Initialize();

	AppendDashLine();
	AppendLine(TestName);
	AppendDashLine();

	for (dword i=0;i<Tests.Count;i++)
	{
		TTest* curTest = Tests[i];
		if ( !(curTest->TestType & tests))
			continue;


		Append("    + ");
		AppendPadded(curTest->TestName,30);

		curTest->Test();

		if (curTest->TestType == TEST_CHECK)
		{
			TTestCheck* curCheck = (TTestCheck*)curTest;
			Append("Pass:");
			AppendPadded( Convert::ToString(curCheck->CountPass) , 5 );
			Append("Fail:");
			AppendPadded( Convert::ToString(curCheck->CountFail) , 5 );
			Append("Total:");
			AppendPadded( Convert::ToString(curCheck->CountTotal) , 5 );

			if (curCheck->CountFail > 0 || ( curCheck->CountOutput > 0 && PrintOutputs ) )
			{
				AppendNewLine();
				for (dword k =0;k<curCheck->Results.Count;k++)
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
			throw NotImplementedException();
		}
		else if (curTest->TestType == TEST_COMPARISON)
		{
			throw NotImplementedException();
		}
		AppendNewLine();
	}

	AppendDashLine();
}