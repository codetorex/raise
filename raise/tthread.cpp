#include "stdafx.h"
#include "tthread.h"


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
}

#endif


