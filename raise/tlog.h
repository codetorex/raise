#ifndef TLOG_H
#define TLOG_H

#include "tstring.h"
#include "tcolor.h"
#include "tcriticalsection.h"

class TLogEntry
{
public:
	String		Content;
	TColor32	Color;
	ui32		GroupID;
	ui32		Tick; // represents relative time from app start
};

class TLogGroup
{
public:
	String		Name;
	String		ShortName;
	ui32		ID;
	TColor32	DefaultColor;
};

enum TLogGroupDefault
{
	LG_FTL,
	LG_ERR,
	LG_WRN,
	LG_INF,
	LG_DBG,
	LG_TRC,
};

/**
 * Observer for log.
 */
class TLogOutput
{
public:
	virtual void Output(TLogEntry* entry) = 0;
};

class TLog
{
private:
	TCriticalSection LogCSection;

	void Notify(TLogEntry* entry)
	{
		LogCSection.Lock();
		for (ui32 i=0;i<Outputs.Count;i++)
		{
			TLogOutput* curOutput = Outputs.Items[i];
			curOutput->Output(entry);
		}
		LogCSection.Unlock();
	}

public:
	Array<TLogOutput*> Outputs;
	Array<TLogGroup*> Groups;

	ui32 StartTick;
	
	TLog();

	inline void RegisterOutput(TLogOutput* output)
	{
		Outputs.Add(output);
	}

	inline void UnregisterOutput(TLogOutput* output)
	{
		Outputs.Remove(output);
	}

	void Output(int group, const String& value);

	inline void Output(int group, const String& fmt, sfp arg0)
	{
		String result = String::Format(fmt,arg0);
		Output(group,result);
	}

	inline void Output(int group, const String& fmt, sfp arg0, sfp arg1)
	{
		String result = String::Format(fmt,arg0,arg1);
		Output(group,result);
	}

	inline void Output(int group, const String& fmt, sfp arg0, sfp arg1,sfp arg2)
	{
		String result = String::Format(fmt,arg0,arg1,arg2);
		Output(group,result);
	}

	inline void Output(int group, const String& fmt, sfp arg0, sfp arg1,sfp arg2,sfp arg3)
	{
		String result = String::Format(fmt,arg0,arg1,arg2,arg3);
		Output(group,result);
	}

	//void Output(int group, const TString& valuefmt, ...);
	
	inline void Output(TLogEntry* entry)
	{
		Notify(entry);
	}
};

extern TLog Log;

#endif