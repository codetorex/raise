#ifndef TTIMING_H
#define TTIMING_H


#ifdef WIN32
#include "stdafx.h"

dword TickCount()
{
	return GetTickCount();
}

qword HPCount() // high precision timer
{
	qword res;
	QueryPerformanceCounter((LARGE_INTEGER*)&res);
	return res;
}

qword RDTSCCount()
{
	qword res;
	_asm
	{
		rdtsc
		mov res.low,eax
		mov res.high,edx
	}
	return res;
}

#else

#include <sys/times.h>

dword TickCount()
{
	tms tm;
	return times(&tm);
}


#endif

#endif