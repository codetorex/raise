#include "stdafx.h"
#include "tlogstream.h"
#include "tapplication.h"
#include "tthread.h"
#include "tstring.h"
#include "tdatetime.h"
#include "tplatform.h"


void TLogStream::Initialize(TStream* outputStream)
{
	Writer.Initialize(outputStream);
	WriteTicks = true;
	WriteGroupName = true;
	WriteThreadId = true;

	TStringBuilderStack<512> sb;

	sb.AppendChar('-',50);
	sb.AppendLine();

	sb.Append("Date: ");
	
	TDateTimeExpanded now = Platform.GetUtcTime();

	sb.AppendLine( now.ToStringUniversal() );

	sb.AppendLine(Application.IdentifyText);

	for (ui32 i=0;i<Application.Modules.Count;i++)
	{
		TModule* curMod = Application.Modules.Items[i];
		sb.Append("    + ");
		sb.AppendLine(curMod->IdentifyText);
	}

	sb.AppendChar('-',50);
	sb.AppendLine();

	Writer.Write(sb);
}

void TLogStream::Output(TLogEntry* entry)
{
	TStringBuilderStack<512> sb;


	if (WriteGroupName)
	{
		sb.Append(Log.Groups.Items[entry->GroupID]->ShortName);
		sb.AppendChar('|');
		sb.AppendChar(' ');
	}

	if (WriteThreadId)
	{
		sb.Append(sfx(TThread::get_CurrentThreadID(),-6));
		sb.AppendChar('|');
		sb.AppendChar(' ');
	}

	if (WriteTicks)
	{;
		sb.AppendPadded(entry->Tick,-10,' ');
		sb.AppendChar('|');
		sb.AppendChar(' ');
	}

	sb.Append(entry->Content);
	sb.AppendLine();

	Writer.Write(sb);

	Writer.BaseStream->Flush();
}

String TLogStream::GetLogFileName( const String& prefix )
{
	TStringBuilderStack<48> sb;

	sb.Append(prefix);

	TDateTimeExpanded now = Platform.GetUtcTime();

	sb.Append(now.ToStringFileName());
	sb.Append(".txt");

	String resultstr = sb.ToString();
	return resultstr;
}
