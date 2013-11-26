#ifndef TWINDOWHANDLE_H
#define TWINDOWHANDLE_H

#include "tstring.h"
#include "tarray.h"

#ifdef WIN32

class TWindowHandle
{
public:
	TWindowHandle(HWND hwnd)
	{
		Handle = hwnd;
	}

	HWND Handle;
	String Title;
	String ClassName;

	void UpdateTitle();
	void UpdateClassName();

	String GetModulePath();
	
	void Update()
	{
		UpdateTitle();
		UpdateClassName();
	}
};

class TWindowList
{
public:
	Array<TWindowHandle*> Items;
	
	~TWindowList()
	{
		Items.DeletePointers();
	}

	void CaptureWindowList();

	/**
	 * Win32 Callback for window item
	 */
	static int _stdcall AddItem(HWND itm, LPARAM prm);
};

#endif


#endif