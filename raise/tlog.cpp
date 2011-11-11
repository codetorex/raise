#include "stdafx.h"
#include "tlog.h"
#include "ttimedriver.h"



TLog::TLog()
{
	StartTick = TimeDriver::TickCount();

	TLogGroup* fatalGroup = new TLogGroup();
	fatalGroup->Name = "Fatal";
	fatalGroup->ID = LG_FTL;
	fatalGroup->DefaultColor.Set24Bit(0xFF0000);
	Groups.Add(fatalGroup);

	TLogGroup* errorGroup = new TLogGroup();
	errorGroup->Name = "Error";
	errorGroup->ID = LG_ERR;
	errorGroup->DefaultColor.Set24Bit(0xDC143C);
	Groups.Add(errorGroup);

	TLogGroup* warnGroup = new TLogGroup();
	warnGroup->Name = "Warning";
	warnGroup->ID = LG_WRN;
	warnGroup->DefaultColor.Set24Bit(0xFFD700);
	Groups.Add(warnGroup);

	TLogGroup* infoGroup = new TLogGroup();
	infoGroup->Name = "Info";
	infoGroup->ID = LG_INF;
	infoGroup->DefaultColor.Set24Bit(0x1E90FF);
	Groups.Add(infoGroup);

	TLogGroup* debugGroup = new TLogGroup();
	debugGroup->Name = "Debug";
	debugGroup->ID = LG_DBG;
	Groups.Add(debugGroup);

	TLogGroup* traceGroup = new TLogGroup();
	traceGroup->Name = "Trace";
	traceGroup->ID = LG_TRC;
	traceGroup->DefaultColor.Set24Bit(0x778899);
	Groups.Add(traceGroup);
}

void TLog::Output( int group, const TString& value )
{
	TLogEntry curEntry;
	curEntry.Content = value;
	curEntry.GroupID = group;
	curEntry.Color = Groups.Item[group]->DefaultColor;
	curEntry.Tick = TimeDriver::TickCount() - StartTick;

	Notify(&curEntry);
}

/*void TLog::OutputFormatted( int group, const TString& fmt, ... )
{

}*/