#include "stdafx.h"
#include "nserverwindows.h"
#include "texception.h"
#include "tlog.h"

/*void NServerWindows::Cleanup()
{
	if(CleanupEvent[0] != WSA_INVALID_EVENT) 
	{
		WSACloseEvent(CleanupEvent[0]);
		CleanupEvent[0] = WSA_INVALID_EVENT;
	}
}

void NServerWindows::SetupServer( NProtocol protocol, const TIPAddress& ip, ui16 port )
{
	SYSTEM_INFO systemInfo;
	WSADATA wsaData;
	int nRet = 0;

	ServerProtocol = protocol;
	OutputDeviceIP = ip;
	ServerPort = port;
	IOCPHandle = INVALID_HANDLE_VALUE;
	Restart = true;
	
	GetSystemInfo(&systemInfo);
	BestWorkerCount = systemInfo.dwNumberOfProcessors * 2;

	if(WSA_INVALID_EVENT == (CleanupEvent[0] = WSACreateEvent()))
	{
		throw Exception("WSACreateEvent() failed: %d\n" ); //, WSAGetLastError()); // NOT THROW BUT LOG
		return;
	}

	if( (nRet = WSAStartup(0x202, &wsaData)) != 0 ) 
	{
		throw Exception("WSAStartup() failed: %d\n"); // ,nRet);  TODO: fix these codes, move to log functions? or debug output?
		Cleanup();
		return;
	}

	try
	{
		ContextListCS.Initialize();
	}
	catch(int a)
	{
		throw Exception("InitializeCriticalSection raised an exception.\n"); // DNOT THROW BUT LOG THESE
		Cleanup();
		return;
	}


}

void NServerWindows::StartServer()
{
	MainThread = new TThread(GetHandler(this,&NServerWindows::MainThreadFunction));
}

void NServerWindows::MainThreadFunction()
{
	while( Restart ) {
		Restart = false;
		EndServer = false;
		WSAResetEvent(CleanupEvent[0]);

		try	
		{

			//
			// notice that we will create more worker threads (dwThreadCount) than 
			// the thread concurrency limit on the IOCP.
			//
			IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if( IOCPHandle == NULL ) 
			{
				//myprintf("CreateIoCompletionPort() failed to create I/O completion port: %d\n", GetLastError());
				throw 0;
			}

			for( ui32 i=0; i< BestWorkerCount; i++ ) 
			{
				TThread* worker = new TThread(GetHandler(this,&NServerWindows::WorkerThreadFunction));
				worker->Start();
				WorkerThreads.Add(worker);
			}

			if( !CreateListenSocket() )
			{
				throw Exception("Creation of listen socket failed");
			}

			if( !CreateAcceptSocket(true) )
			{
				throw Exception("Creation of accept socket failed");
			}

			WSAWaitForMultipleEvents(1, CleanupEvent, TRUE, WSA_INFINITE, FALSE);
		}
		catch(Exception& exc)
		{
			// LOG THE ERROR
		}


		EndServer = true;

		//
		// Cause worker threads to exit
		//
		if( IOCPHandle ) 
		{
			for( ui32 i = 0; i < WorkerThreads.Count; i++ )
			{
				PostQueuedCompletionStatus(IOCPHandle, 0, 0, NULL);
			}
		}

		HANDLE tHandles[64];
		for (int i=0;i<WorkerThreads.Count;i++)
		{
			tHandles[i] = WorkerThreads.Item[i]->ThreadHandle;
		}

		//
		// Make sure worker threads exits.
		//
		if( WAIT_OBJECT_0 != WaitForMultipleObjects(WorkerThreads.Count,  tHandles, TRUE, 1000) )
		{
			Log.OutputFormatted(LG_ERR,"WaitForMultipleObjects() failed: %", sfu(GetLastError()));
		}
		else
		{
			for( ui32 i=0; i< WorkerThreads.Count ; i++ ) 
			{
				TThread* curThread = WorkerThreads.Item[i];
				if( tHandles[i] != INVALID_HANDLE_VALUE )
				{
					CloseHandle(tHandles[i]);
				}
				curThread->Abort();
				curThread->ThreadHandle = INVALID_HANDLE_VALUE;
				tHandles[i] = INVALID_HANDLE_VALUE;
			}
		}

		if( ListenSocket != INVALID_SOCKET ) 
		{
			closesocket(ListenSocket);                                
			ListenSocket = INVALID_SOCKET;
		}

		if( g_pCtxtListenSocket ) 
		{
			while( !HasOverlappedIoCompleted((LPOVERLAPPED)&g_pCtxtListenSocket->pIOContext->Overlapped) )
				Sleep(0);

			if( g_pCtxtListenSocket->pIOContext->SocketAccept != INVALID_SOCKET )
				closesocket(g_pCtxtListenSocket->pIOContext->SocketAccept);
			g_pCtxtListenSocket->pIOContext->SocketAccept = INVALID_SOCKET;

			//
			// We know there is only one overlapped I/O on the listening socket
			//
			if( g_pCtxtListenSocket->pIOContext )
				xfree(g_pCtxtListenSocket->pIOContext);

			if( g_pCtxtListenSocket )
				xfree(g_pCtxtListenSocket);
			g_pCtxtListenSocket = NULL;
		}

		CtxtListFree();

		if( IOCPHandle ) 
		{
			CloseHandle(IOCPHandle);
			IOCPHandle = NULL;
		}

		if( Restart ) 
		{
			myprintf("\niocpserverex is restarting...\n");
		} 
		else
		{
			myprintf("\niocpserverex is exiting...\n");
		}

	} //while (g_bRestart)

	ContextListCS.Finalize();
	Cleanup();
	WSACleanup();
}

void NServerWindows::WorkerThreadFunction()
{

}

SOCKET NServerWindows::CreateSocket()
{
	int nRet = 0;
	int nZero = 0;
	SOCKET sdSocket = INVALID_SOCKET;

	sdSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED); 
	if( sdSocket == INVALID_SOCKET ) 
	{
		//myprintf("WSASocket(sdSocket) failed: %d\n", WSAGetLastError());
		return(sdSocket);
	}

    //
	// Disable send buffering on the socket.  Setting SO_SNDBUF
	// to 0 causes winsock to stop buffering sends and perform
	// sends directly from our buffers, thereby save one memory copy.
	//
    // However, this does prevent the socket from ever filling the
    // send pipeline. This can lead to packets being sent that are
    // not full (i.e. the overhead of the IP and TCP headers is 
    // great compared to the amount of data being carried).
    //
    // Disabling the send buffer has less serious repercussions 
    // than disabling the receive buffer.
	//
	nZero = 0;
	nRet = setsockopt(sdSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	if( nRet == SOCKET_ERROR) 
	{
		//myprintf("setsockopt(SNDBUF) failed: %d\n", WSAGetLastError());
		return(sdSocket);
	}

    //
    // Don't disable receive buffering. This will cause poor network
    // performance since if no receive is posted and no receive buffers,
    // the TCP stack will set the window size to zero and the peer will
    // no longer be allowed to send data.
    //

    // 
    // Do not set a linger value...especially don't set it to an abortive
    // close. If you set abortive close and there happens to be a bit of
    // data remaining to be transfered (or data that has not been 
    // acknowledged by the peer), the connection will be forcefully reset
    // and will lead to a loss of data (i.e. the peer won't get the last
    // bit of data). This is BAD. If you are worried about malicious
    // clients connecting and then not sending or receiving, the server
    // should maintain a timer on each connection. If after some point,
    // the server deems a connection is "stale" it can then set linger
    // to be abortive and close the connection.
    //*/

    /*
	LINGER lingerStruct;

	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;
	nRet = setsockopt(sdSocket, SOL_SOCKET, SO_LINGER,
					  (char *)&lingerStruct, sizeof(lingerStruct));
	if( nRet == SOCKET_ERROR ) {
		myprintf("setsockopt(SO_LINGER) failed: %d\n", WSAGetLastError());
		return(sdSocket);
	}
    */

/*	return(sdSocket);
}

bool NServerWindows::CreateListenSocket()
{
	int nRet = 0;
	LINGER lingerStruct;
	struct addrinfo hints = {0};
	struct addrinfo *addrlocal = NULL;

	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;

	//
	// Resolve the interface
	//
	hints.ai_flags  = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_IP;

	TString portstr = TConvert::ToString(ServerPort);

	if( getaddrinfo(NULL, (char*)portstr.Data, &hints, &addrlocal) != 0 ) 
	{
		myprintf("getaddrinfo() failed with error %d\n", WSAGetLastError());
		return(FALSE);
	}

	if( addrlocal == NULL ) 
	{
		myprintf("getaddrinfo() failed to resolve/convert the interface\n");
		return(FALSE);
	}

	ListenSocket = CreateSocket();
	if( ListenSocket == INVALID_SOCKET) {
		freeaddrinfo(addrlocal);
		return(FALSE);
	}

	nRet = bind(ListenSocket, addrlocal->ai_addr, (int) addrlocal->ai_addrlen);
	if( nRet == SOCKET_ERROR) {
		myprintf("bind() failed: %d\n", WSAGetLastError());
		freeaddrinfo(addrlocal);
		return(FALSE);
	}

	nRet = listen(ListenSocket, 5);
	if( nRet == SOCKET_ERROR ) {
		myprintf("listen() failed: %d\n", WSAGetLastError());
		freeaddrinfo(addrlocal);
		return(FALSE);
	}

	freeaddrinfo(addrlocal);

	return true;
}
*/