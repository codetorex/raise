#include "stdafx.h"
#include "tcommandline.h"

//TArray<TCommandLineHandler*> TCommandLineHandler::AllHandlers;

void TCommandLine::ParseCommandLine()
{
	for (ParameterIndex=0;ParameterIndex<Parameters.Count;)
	{
		TCommandLineHandler::HandleResult Result = TCommandLineHandler::HR_Continue;

		for (int h=0;h<Handlers.Count;h++)
		{
			Result = Handlers.Item[ParameterIndex]->HandleParameter(this);
			if ( Result == TCommandLineHandler::HR_Handled )
			{
				MoveNextParameter();
				break;
			}
		}

		if (Result == TCommandLineHandler::HR_Continue )
		{
			throw Exception("Unhandled command line parameter '%'", sfs(*Parameters[ParameterIndex]));
		}
	}
}

/*void TCommandLine::RegisterAllHandlers()
{
	for (ui32 i =0;i<TCommandLineHandler::AllHandlers.Count;i++)
	{
		Handlers.Add(TCommandLineHandler::AllHandlers.Item[i]);
	}
}*/

void TCommandLine::RegisterHandler( TCommandLineHandler* Handler )
{
	Handlers.Add(Handler);
}

TCommandLineHandler::HandleResult TCommandLineHandlerSimple::HandleParameter( TCommandLine* cmd )
{
	TString& curPrm = cmd->GetCurrentParameter();
	if (curPrm == Parameter || curPrm == ShortParameter)
	{
		ProcessParameter(cmd);
		return HR_Handled;
	}

	return HR_Continue;
}