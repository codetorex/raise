#include "stdafx.h"


#ifdef WIN32

#include "tplatformwin32.h"
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

#endif

