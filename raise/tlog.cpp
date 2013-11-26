#include "stdafx.h"
#include "tlog.h"
#include "tplatform.h"

TLog Log;

TLog::TLog()
{
	LogCSection.Initialize();

	StartTick = Platform.TickCount();

	TLogGroup* fatalGroup = new TLogGroup();
	fatalGroup->Name = "Fatal";
	fatalGroup->ShortName = "FTL";
	fatalGroup->ID = LG_FTL;
	fatalGroup->DefaultColor.Set24Bit(0xFF0000);
	Groups.Add(fatalGroup);

	TLogGroup* errorGroup = new TLogGroup();
	errorGroup->Name = "Error";
	errorGroup->ShortName = "ERR";
	errorGroup->ID = LG_ERR;
	errorGroup->DefaultColor.Set24Bit(0xDC143C);
	Groups.Add(errorGroup);

	TLogGroup* warnGroup = new TLogGroup();
	warnGroup->Name = "Warning";
	warnGroup->ShortName = "WRN";
	warnGroup->ID = LG_WRN;
	warnGroup->DefaultColor.Set24Bit(0xFFD700);
	Groups.Add(warnGroup);

	TLogGroup* infoGroup = new TLogGroup();
	infoGroup->Name = "Info";
	infoGroup->ShortName = "INF";
	infoGroup->ID = LG_INF;
	infoGroup->DefaultColor.Set24Bit(0x1E90FF);
	Groups.Add(infoGroup);

	TLogGroup* debugGroup = new TLogGroup();
	debugGroup->Name = "Debug";
	debugGroup->ShortName = "DBG";
	debugGroup->ID = LG_DBG;
	Groups.Add(debugGroup);

	TLogGroup* traceGroup = new TLogGroup();
	traceGroup->Name = "Trace";
	traceGroup->ShortName = "TRC";
	traceGroup->ID = LG_TRC;
	traceGroup->DefaultColor.Set24Bit(0x778899);
	Groups.Add(traceGroup);
}

void TLog::Output( int group, const String& value )
{
	TLogEntry curEntry;
	curEntry.Content = value;
	curEntry.GroupID = group;
	curEntry.Color = Groups.Items[group]->DefaultColor;
	curEntry.Tick = Platform.TickCount() - StartTick;

	Notify(&curEntry);
}

/*void TLog::OutputFormatted( int group, const TString& fmt, ... )
{

}*/