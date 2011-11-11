#include "stdafx.h"
#include "tthread.h"
#include "texception.h"

#ifdef WIN32

DWORD WINAPI WinThread (LPVOID lpdwThreadParam ) 
{
	TThread* myThread = (TThread*)lpdwThreadParam;
	myThread->IsAlive = true;

	// TODO: change status here

	if (myThread->IsParamterized)
	{
		myThread->PStart->call(myThread->ParameterObject);
	}
	else
	{
		myThread->TStart->call();
	}

	myThread->IsAlive = false;
	return 0;
}

void TThread::Start()
{
	ThreadHandle = CreateThread(NULL,MaximumStackSize,WinThread,this,NULL,(LPDWORD)&ID);
	if (ThreadHandle == NULL)
	{
		throw Exception("Thread creation failed");
	}
}

ui32 TThread::get_CurrentThreadID()
{
	return GetCurrentThreadId();
}

#else

ui32 TThread::get_CurrentThreadID()
{
	//http://www.kernel.org/doc/man-pages/online/pages/man2/gettid.2.html
	return gettid();
}

#endif


