#ifndef TLOGSTREAM_H
#define TLOGSTREAM_H

#include "tlog.h"
#include "tstreamwriterutf8.h"

class TLogStream: public TLogOutput
{
public:
	TStreamWriterUTF8 Writer;

	bool WriteTicks;
	bool WriteGroupName;
	bool WriteThreadId; // good for multi-threaded debug?

	TLogStream()
	{
		
	}

	void Initialize(TStream* outputStream);

	TLogStream(TStream* outputStream)
	{
		Initialize(outputStream);
	}

	void Output(TLogEntry* entry);

	/**
	 * Returns a file name like log20111113215629.txt
	 * rest of is date
	 */
	static TString GetLogFileName(const TString& prefix);
};

#endif