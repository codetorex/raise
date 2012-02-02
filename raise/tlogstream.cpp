#include "stdafx.h"
#include "tlogstream.h"
#include "tapplication.h"
#include "tthread.h"
#include "tstringbuilder.h"
#include "tdatetime.h"
#include "ttimedriver.h"


void TLogStream::Initialize(TStream* outputStream)
{
	Writer.Initialize(outputStream);
	WriteTicks = true;
	WriteGroupName = true;
	WriteThreadId = true;

	byte stackbuffer[512];
	TStringBuilder sb(stackbuffer,512);
	sb.AppendChar('-',50);
	sb.AppendLine();

	sb.Append("Date: ");
	TDateTimeExpanded now = TimeDriver::Now();
	sb.AppendLine( now.ToStringUniversal() );

	sb.AppendLine(Application.IdentifyText);

	for (ui32 i=0;i<Application.Modules.Count;i++)
	{
		TModule* curMod = Application.Modules.Item[i];
		sb.Append("    + ");
		sb.AppendLine(curMod->IdentifyText);
	}

	sb.AppendChar('-',50);
	sb.AppendLine();

	Writer.Write(sb);
	sb.UnbindByteArray();
}

void TLogStream::Output(TLogEntry* entry)
{
	byte stackbuffer[512];

	TStringBuilder sb(stackbuffer,512);
	bool UsingStack = true;
	if (entry->Content.ByteLength > 384)
	{
		UsingStack = false;
		sb.UnbindByteArray();
		sb.InitializeCapacity(entry->Content.ByteLength+128);
	}

	if (WriteGroupName)
	{
		sb.Append(Log.Groups.Item[entry->GroupID]->ShortName);
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

	if (UsingStack)
	{
		sb.UnbindByteArray();
	}
}

TString TLogStream::GetLogFileName( const TString& prefix )
{
	byte stackbuffer[48];
	TStringBuilder sb(stackbuffer,48);

	sb.Append(prefix);
	TDateTimeExpanded now = TimeDriver::Now();
	sb.Append(now.ToStringFileName());
	sb.Append(".txt");

	TString resultstr = sb.ToString();
	sb.UnbindByteArray();

	return resultstr;
}
