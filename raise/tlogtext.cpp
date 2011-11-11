#include "stdafx.h"
#include "tlogtext.h"
#include "tapplication.h"
#include "tthread.h"
#include "tstringbuilder.h"


void TLogText::Initialize(TStream* outputStream)
{
	Writer.Initialize(outputStream);
	WriteTicks = true;
	WriteGroupName = true;
	WriteThreadId = true;

	byte stackbuffer[512];
	TStringBuilder sb(stackbuffer,512);
	sb.AppendChar('-',25);
	sb.AppendLine();
	sb.Append(Application.Name);
	sb.Append(" - ");
	sb.Append(Application.Version);
	sb.Append(" - ");
	sb.Append(Application.Company);
	sb.AppendLine();
	sb.AppendChar('-',25);
	sb.AppendLine();

	Writer.Write(sb);
	sb.UnbindByteArray();
}

void TLogText::Output(TLogEntry* entry)
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

	if (WriteTicks)
	{
		sb.AppendPadded(entry->Tick,16,' ');
		sb.AppendChar(' ');
		sb.AppendChar('|');
	}

	if (WriteGroupName)
	{
		sb.AppendPadded(Log.Groups.Item[entry->GroupID]->Name,8,' ',true);
		sb.AppendChar(' ');
		sb.AppendChar('|');
	}

	if (WriteThreadId)
	{
		sb.AppendPadded(TThread::get_CurrentThreadID(),6);
		sb.AppendChar(' ');
		sb.AppendChar('|');
	}

	sb.AppendChar(' ');
	sb.Append(entry->Content);
	sb.AppendLine();

	Writer.Write(sb);

	if (UsingStack)
	{
		sb.UnbindByteArray();
	}
}

