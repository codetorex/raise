#include "stdafx.h"
#include "tprocess.h"
#include "tplatform.h"

#ifdef WIN32

void TWindowList::CaptureWindowList()
{
	WNDENUMPROC enumFunc = TWindowList::AddItem;
	LPARAM prm = (LPARAM)this;

	BOOL r = EnumWindows(enumFunc, prm);
}

int TWindowList::AddItem( HWND itm, LPARAM prm )
{

	TWindowList* wl = (TWindowList*)prm;

	TWindowHandle* newItem = new TWindowHandle(itm);
	newItem->Update();

	wl->Items.Add(newItem);

	return TRUE;
}


void TWindowHandle::UpdateTitle()
{
	ch16 Temp[4096];
	Temp[0] = 0;
	GetWindowTextW(Handle,Temp,4096);
	Title = Platform.RaiseString(Temp);
}

void TWindowHandle::UpdateClassName()
{
	ch16 Temp[4096];
	Temp[0] = 0;
	GetClassNameW(Handle,Temp,4096);
	ClassName = Platform.RaiseString(Temp);
}

String TWindowHandle::GetModulePath()
{
	TProcess prc;
	try
	{
		prc = TProcessManager::FindByWindow(*this);
		prc.OpenProcessHandle();
		String result = prc.GetModulePath();
		prc.CloseProcessHandle();
		return result;
	}
	catch(Exception& e)
	{

	}

	return String::Empty;
}

#endif