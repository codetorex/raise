#include "stdafx.h"
#include "tprocess.h"
#include "twintools.h"

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
	Title = TWinTools::RaiseString(Temp);
}

void TWindowHandle::UpdateClassName()
{
	ch16 Temp[4096];
	Temp[0] = 0;
	GetClassNameW(Handle,Temp,4096);
	ClassName = TWinTools::RaiseString(Temp);
}

TString TWindowHandle::GetModulePath()
{
	TProcess prc;
	try
	{
		prc = TProcessManager::FindByWindow(*this);
		prc.OpenProcessHandle();
		TString result = prc.GetModulePath();
		prc.CloseProcessHandle();
		return result;
	}
	catch(Exception& e)
	{

	}

	return TString::Empty;
}
