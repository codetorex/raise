#include "stdafx.h"
#include "tprocess.h"
#include "tplatform.h"

#ifdef WIN32

TProcess TProcessManager::FindByWindowName( const String& windowname )
{
	if (!windowname.IsASCII())
	{
		throw Exception("Window name has unicode characters");
	}
	HWND wnd = FindWindowA(0,(char*)windowname.Data);
	if (!wnd)
	{
		throw Exception("Can't find window");
	}

	return FindByWindow(wnd);
}

TProcess TProcessManager::FindByClassName( const String& className )
{
	if (!className.IsASCII())
	{
		throw Exception("Window name has unicode characters");
	}
	HWND wnd = FindWindowA((char*)className.Data,0);
	if (!wnd)
	{
		throw Exception("Can't find window");
	}

	return FindByWindow(wnd);
}

TProcess TProcessManager::FindByWindow( const TWindowHandle& winHandle )
{
	DWORD processID;
	GetWindowThreadProcessId(winHandle.Handle,&processID);
	if (!processID)
	{
		throw Exception("Can't get process id");
	}

	return TProcess(processID);
}

TProcess TProcessManager::CreateFromExecutable( const String& exepath, const String& params /*= 0*/ )
{
	throw NotImplementedException(__FILE__,__LINE__);
	return TProcess(0);
}

TProcess TProcessManager::FindByWindowNamePart( const String& windowname )
{
	throw NotImplementedException(__FILE__,__LINE__);
	return TProcess(0);
}

TProcess TProcessManager::FindByWindowNamePartExePathPart( const String& windownamePart, const String& exenamePart )
{
	TWindowList windows;
	windows.CaptureWindowList();

	TWindowHandle* foundItem = 0;

	TArrayEnumerator<TWindowHandle*> we(windows.Items); // window enumerator
	while(we.MoveNext())
	{
		TWindowHandle* win = we.Current;
		if (win->Title.IndexOf(windownamePart) > 0)
		{
			String windowExe = win->GetModulePath();
			if (windowExe.IndexOf(exenamePart) > 0)
			{
				foundItem = win;
				break;
			}
		}
	}

	if (!foundItem)
	{
		throw Exception("Couldn't find window");
	}

	return FindByWindow(foundItem->Handle);
}

#endif