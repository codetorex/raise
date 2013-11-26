#ifndef TPLATFORMINTERFACE_H
#define TPLATFORMINTERFACE_H

#include "tstring.h"
#include "m64bit.h"
#include "tdatetime.h"

enum EDialogResult
{
	TDR_None,
	TDR_OK,
	TDR_Cancel,
	TDR_Abort,
	TDR_Retry,
	TDR_Ignore,
	TDR_Yes,
	TDR_No,
};

class TDateTime;

/**
 * With this class we are setting functions needed to be implemented by target platforms.
 */
class IPlatform
{
public:
	virtual void RaiseToSystemString(const String& raiseStr, byte* output, ui32 outputSize) = 0;
	virtual String SystemToRaiseString(byte* input, ui32 inputSize) = 0;

	virtual ui32 TickCount() = 0;
	virtual TDateTime GetUtcTime() = 0;
	virtual TDateTime ToLocalTime(const TDateTime& inputDateTime) = 0;

	virtual qword HighPrecisionTime() = 0;
	virtual qword HighPrecisionTimeFrequency() = 0;


	// The functions that I am not sure about their correctness comes here
	virtual void ShowMessageBox(const String& message) = 0;
	virtual String GetErrorDescription(ui32 systemErrorID) = 0;
	virtual bool BrowseFolder(const String& startPath, String& selectedPath) = 0;
};

#endif