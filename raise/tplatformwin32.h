#ifndef TPLATFORMWIN32_H
#define TPLATFORMWIN32_H

#ifdef WIN32

#include "tplatforminterface.h"
#include "tencodings.h"

class TPlatformWin32: public IPlatform
{
public:

	inline void RaiseToSystemString( const TString& raiseStr, byte* output, ui32 outputSize ) 
	{
		Encodings.UTF16.ConvertString(raiseStr,output,outputSize);
	}

	inline TString SystemToRaiseString( byte* input, ui32 inputSize ) 
	{
		return Encodings.UTF16.GetString(input,inputSize);
	}

	void ShowMessageBox( const TString& message );

	inline TString RaiseString(const ch16* systemStr)
	{
		return Encodings.UTF16.GetString(systemStr);
	}

	TString GetErrorDescription( ui32 systemErrorID );

	bool BrowseFolder( const TString& startPath, TString& selectedPath );

	TDateTime GetUtcTime();

	TDateTime ToLocalTime( const TDateTime& inputDateTime );

	inline ui32 TickCount()
	{
		return GetTickCount();
	}

	qword HighPrecisionTime();

	qword HighPrecisionTimeFrequency();

};

typedef TPlatformWin32 TPlatformCurrent;

#endif // WIN32

#endif