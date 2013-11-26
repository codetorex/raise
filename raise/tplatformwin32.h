#ifndef TPLATFORMWIN32_H
#define TPLATFORMWIN32_H

#ifdef WIN32

#include "tplatforminterface.h"
#include "tencodings.h"

class TPlatformWin32: public IPlatform
{
public:

	inline void RaiseToSystemString( const String& raiseStr, byte* output, ui32 outputSize ) 
	{
		Encodings.UTF16.ConvertString(raiseStr,output,outputSize);
	}

	inline String SystemToRaiseString( byte* input, ui32 inputSize ) 
	{
		return Encodings.UTF16.GetString(input,inputSize);
	}

	void ShowMessageBox( const String& message );

	inline String RaiseString(const ch16* systemStr)
	{
		return Encodings.UTF16.GetString(systemStr);
	}

	String GetErrorDescription( ui32 systemErrorID );

	bool BrowseFolder( const String& startPath, String& selectedPath );

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