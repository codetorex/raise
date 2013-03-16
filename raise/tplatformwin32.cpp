#include "stdafx.h"


#ifdef WIN32

#include "tplatform.h"
#include "tdatetimewindows.h"
#include "tdatetime.h"
#include <ShlObj.h>

bool TPlatformWin32::BrowseFolder( const TString& startPath, TString& selectedPath )
{
	bool result = false;

	BROWSEINFO bi;
	MemoryDriver::Set(&bi,0,sizeof(bi));

	bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
	
	OleInitialize(NULL);

	LPITEMIDLIST pIDL = SHBrowseForFolder(&bi);

	if (pIDL != NULL)
	{
		ch16 buffer[4096];
		if (SHGetPathFromIDList(pIDL,buffer))
		{
			result = true;
			selectedPath = RaiseString(buffer);
		}

		CoTaskMemFree(pIDL);
	}

	OleUninitialize();

	return result;

	/*LPITEMIDLIST pidl     = NULL;
	BROWSEINFO   bi       = { 0 };
	BOOL         bResult  = FALSE;

	bi.hwndOwner      = hWnd;
	bi.pszDisplayName = szBuf;
	bi.pidlRoot       = NULL;
	bi.lpszTitle      = szTitle;
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
	bResult = SHGetPathFromIDList(pidl, szBuf);
	CoTaskMemFree(pidl);
	}

	return bResult;*/
}

void TPlatformWin32::ShowMessageBox( const TString& message )
{
	ch16 Temp[4096];
	RaiseToSystemString(message,(byte*)Temp,sizeof(Temp));

	MessageBoxW(0,Temp,L"Message", MB_OK);
}

TString TPlatformWin32::GetErrorDescription( ui32 systemErrorID )
{
	ch16 Temp[4096];

	FormatMessageW( 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		systemErrorID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		Temp,
		4096, NULL );

	return RaiseString(Temp);
}


TDateTime TPlatformWin32::GetUtcTime()
{
	TDateTimeWindows wintime;
	GetSystemTimeAsFileTime(&wintime.Date);

	return TDateTime( wintime );
}

string LocalStandartName;
string LocalDaylightName;
int LocalDaylight = 0;
ui32 LocalLastUpdate = 0;

// Local time difference in seconds
int LocalDifference = 0;
bool HighPrecisionSupported = true;

void UpdateLocalTimezone()
{
	TIME_ZONE_INFORMATION dtz;
	ui32 status = GetTimeZoneInformation (&dtz);

	if (status == TIME_ZONE_ID_STANDARD || status == TIME_ZONE_ID_UNKNOWN)
	{
		LocalDaylight = 0;
	}
	else if (status == TIME_ZONE_ID_DAYLIGHT)
	{
		LocalDaylight = 1;
		dtz.Bias += dtz.DaylightBias;
	}

	LocalStandartName = Platform.RaiseString(dtz.StandardName);
	LocalDaylightName = Platform.RaiseString(dtz.DaylightName);
	LocalDifference = dtz.Bias * -60; // in seconds

	LocalLastUpdate = Platform.TickCount();
}

TDateTime TPlatformWin32::ToLocalTime( const TDateTime& inputDateTime )
{
	if (TickCount() - LocalLastUpdate > (60 * 60 * 1000)) // 1 hour
	{
		UpdateLocalTimezone(); // Do this every 2 hours or something
	}

	TDateTime result = inputDateTime;
	result.AddSeconds(LocalDifference);
	return result;
}

qword TPlatformWin32::HighPrecisionTime()
{
	qword val;
	if ( !QueryPerformanceCounter((LARGE_INTEGER*)&val) )
	{
		HighPrecisionSupported = false;
		return TickCount();
	}

	return val;
}

qword TPlatformWin32::HighPrecisionTimeFrequency()
{
	if (!HighPrecisionSupported)
		return 1000;

	qword val;
	QueryPerformanceFrequency((LARGE_INTEGER*)&val);
	return val;
}


#endif

