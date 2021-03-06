#ifndef TLOGSTREAM_H
#define TLOGSTREAM_H

#include "tlog.h"
#include "tstreamwriterutf8.h"

/// TODO: TLogTextWriter which will be created from TextWriter based class.

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
	static String GetLogFileName(const String& prefix);
};

#endif