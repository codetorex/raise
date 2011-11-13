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

	sb.AppendLine(Application.IdentifyText);

	for (int i=0;i<Application.Modules.Count;i++)
	{
		TModule* curMod = Application.Modules.Item[i];
		sb.Append("    + ");
		sb.AppendLine(curMod->IdentifyText);
	}

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

