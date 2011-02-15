#include "stdafx.h"
#include "tprocess.h"
#include "texception.h"


void TProcess::OpenFromWindow( const str8& windowname )
{
	HWND wnd = FindWindowA(0,windowname.Chars);
	if (!wnd)
	{
		throw Exception("Can't find window");
	}

	GetWindowThreadProcessId(wnd,&processID);
	if (!processID)
	{
		throw Exception("Can't get process id");
	}

	processHandle = OpenProcess(PROCESS_ALL_ACCESS,0,processID);
	if (!processHandle)
	{
		throw Exception("Can't open process");
	}
}

void TProcess::CreateFromExecutable( const str8& exepath, const str8& params /*= 0*/ )
{
	throw Exception("Not implemented yet");
}

dword TProcessMemory::Search( dword start,dword end, const void* needle, int length )
{
	for (dword addr=start;addr < end; addr += Buffer.Capacity)
	{
		if (FillBuffer(addr) != 0) // 0 bytes readed at that section
		{
			dword found = Buffer.VirtualSearch((byte*)needle,length);
			if (found != NFOUND) return found;
		}
		addr -= length-1;
	}
	return NFOUND;
}

dword TProcessMemory::SearchPattern( dword start,dword end, const void* needle, const byte* pattern, int length )
{
	for (dword addr=start;addr < end; addr += Buffer.Capacity)
	{
		if (FillBuffer(addr) != 0) // 0 bytes readed at that section
		{
			dword found = Buffer.VirtualSearchPattern((byte*)needle,pattern,length);
			if (found != NFOUND) return found;
		}
		addr -= length-1;
	}
	return NFOUND;
}