#ifndef TLOGTEXT_H
#define TLOGTEXT_H

#include "tlog.h"
#include "tstreamwriterutf8.h"

class TLogText: public TLogOutput
{
public:
	TStreamWriterUTF8 Writer;

	bool WriteTicks;
	bool WriteGroupName;
	bool WriteThreadId; // good for multi-threaded debug?

	TLogText()
	{
		
	}

	void Initialize(TStream* outputStream);

	TLogText(TStream* outputStream)
	{
		Initialize(outputStream);
	}

	void Output(TLogEntry* entry);
};

#endif