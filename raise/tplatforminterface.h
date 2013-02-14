#ifndef TPLATFORMINTERFACE_H
#define TPLATFORMINTERFACE_H

#include "tstring.h"

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

/**
 * With this class we are setting functions needed to be implemented by target platforms.
 */
class TPlatformInterface
{
public:
	virtual void RaiseToSystemString(const TString& raiseStr, byte* output, ui32 outputSize) = 0;
	virtual TString SystemToRaiseString(byte* input, ui32 inputSize) = 0;

	virtual void ShowMessageBox(const TString& message) = 0;


	// The functions that I am not sure about their correctness comes here
	virtual TString GetErrorDescription(ui32 systemErrorID) = 0;
	virtual bool BrowseFolder(const TString& startPath, TString& selectedPath) = 0;
};

#endif