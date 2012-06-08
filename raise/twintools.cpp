#include "stdafx.h"
#include "twintools.h"

#include <ShlObj.h>

ch16 TWinTools::TempSystemString[1024];

bool TWinTools::BrowseFolder( const TString& startPath, TString& OUT selectedPath )
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