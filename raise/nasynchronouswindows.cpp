#include "stdafx.h"
#include "nasynchronouswindows.h"
#include "texception.h"
#include "tlog.h"

#ifdef WIN32


void NAsynchronousWindows::Cleanup()
{
	if(CleanupEvent[0] != WSA_INVALID_EVENT) 
	{
		WSACloseEvent(CleanupEvent[0]);
		CleanupEvent[0] = WSA_INVALID_EVENT;
	}
}

void NAsynchronousWindows::StartServer()
{
	MainThread = new TThread(GetHandler(this,&NAsynchronousWindows::MainThreadFunction));
	MainThread->Start();
}

void NAsynchronousWindows::MainThreadFunction()
{
	SYSTEM_INFO systemInfo;
	WSADATA wsaData;
	int nRet = 0;

	IOCPHandle = INVALID_HANDLE_VALUE;
	Restart = true;

	GetSystemInfo(&systemInfo);
	BestWorkerCount = systemInfo.dwNumberOfProcessors * 2;

	if(WSA_INVALID_EVENT == (CleanupEvent[0] = WSACreateEvent()))
	{
		Log.Output(LG_ERR,"WSACreateEvent() failed: %", sfi(WSAGetLastError()));
		return;
	}

	if( (nRet = WSAStartup(0x202, &wsaData)) != 0 ) 
	{
		Log.Output(LG_ERR,"WSAStartup() failed: %", sfi(nRet));
		Cleanup();
		return;
	}

	try
	{
		ContextListCS.Initialize();
	}
	catch(int a)
	{
		Log.Output(LG_ERR,"InitializeCriticalSection raised an exception. %", sfi(a));
		Cleanup();
		return;
	}

	while( Restart ) 
	{
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
				Log.Output(LG_ERR,"CreateIoCompletionPort() failed to create I/O completion port: %", sfu(GetLastError()));
				throw 0;
			}

			for( ui32 i=0; i< BestWorkerCount; i++ ) 
			{
				TThread* worker = new TThread(GetHandler(this,&NAsynchronousWindows::WorkerThreadFunction));
				worker->Start();
				WorkerThreads.Add(worker);
			}

			while(TaskQueue.Count)
			{
				NServerOperation* opr = TaskQueue.Pop();

				switch (opr->Operation)
				{
				case NServerOperation::OT_CREATELISTENER:
					CreateListenerParameters* prms = (CreateListenerParameters*)opr->Parameter;
					CreateListenerTask(prms->Device,prms->Port,prms->Protocol,prms->Service);
					break;
				}

				delete opr;
			}

			/*if( !CreateListenSocket() )
			{
				Log.Output(LG_ERR,"Creation of listen socket failed");
			}

			if( !CreateAcceptSocket(true) )
			{
				Log.Output(LG_ERR,"Creation of accept socket failed");
			}*/

			WSAWaitForMultipleEvents(1, CleanupEvent, TRUE, WSA_INFINITE, FALSE);
		}
		catch(Exception& exc)
		{
			Log.Output(LG_ERR,"Error occurred at server initialization: %", sfs(exc.Message));
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
		for (ui32 i=0;i<WorkerThreads.Count;i++)
		{
			tHandles[i] = WorkerThreads.Item[i]->ThreadHandle;
		}

		//
		// Make sure worker threads exits.
		//
		if( WAIT_OBJECT_0 != WaitForMultipleObjects(WorkerThreads.Count,  tHandles, TRUE, 1000) )
		{
			Log.Output(LG_ERR,"WaitForMultipleObjects() failed: %", sfu(GetLastError()));
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

		/*
		TODO: close all listener sockets
		if( ListenSocket != INVALID_SOCKET ) 
		{
			closesocket(ListenSocket);                                
			ListenSocket = INVALID_SOCKET;
		}*/

		/*
		
		TODO: fix all listener sockets
		
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

		CtxtListFree();*/

		if( IOCPHandle ) 
		{
			CloseHandle(IOCPHandle);
			IOCPHandle = NULL;
		}

		if( Restart ) 
		{
			Log.Output(LG_INF,"Raise server is restarting...");
		} 
		else
		{
			Log.Output(LG_INF,"Raise server is exiting...");
		}

	} //while (g_bRestart)

	ContextListCS.Finalize();
	Cleanup();
	WSACleanup();
}


NSocketWindows* NAsynchronousWindows::AllocateNewSocket( SOCKET sd, PacketIO op )
{
	ContextListCS.Lock();

	NSocketWindows* newSocket = new NSocketWindows();

	newSocket->Socket = sd;

	ContextListCS.Unlock();

	return newSocket;
}

NSocketWindows::NSocketWindows()
{
	IOContextRecv.UseRecvPacket(&ReceiveBuffer);
}

NSocketWindows* NAsynchronousWindows::UpdateCompletionPort( SOCKET sd, PacketIO op, bool addtoList )
{
	NSocketWindows* lpPerSocketContext;

	lpPerSocketContext = AllocateNewSocket(sd, op);
	if( lpPerSocketContext == NULL )
		return NULL;

	IOCPHandle = CreateIoCompletionPort((HANDLE)sd, IOCPHandle, (ULONG_PTR)lpPerSocketContext, 0);
	if(IOCPHandle == NULL) 
	{
		Log.Output(LG_ERR,"CreateIoCompletionPort() failed: %", sfu(GetLastError()));
		delete lpPerSocketContext;
		return NULL;
	}

	//
	//The listening socket context (bAddToList is FALSE) is not added to the list.
	//All other socket contexts are added to the list.
	//
	//if( bAddToList ) CtxtListAddTo(lpPerSocketContext);

	Log.Output(LG_INF,"UpdateCompletionPort: Socket(%) added to IOCP", sfu(lpPerSocketContext->Socket));

	return lpPerSocketContext;
}


bool NAsynchronousWindows::CreateAcceptSocket( NListenerWindows* Listener, bool UpdateIOCP )
{
	int nRet = 0;
	DWORD dwRecvNumBytes = 0;
	DWORD bytes = 0;

	//
	// GUID to Microsoft specific extensions
	//
	GUID acceptex_guid = WSAID_ACCEPTEX;

	//
	//The context for listening socket uses the SockAccept member to store the
	//socket for client connection. 
	//
	if( UpdateIOCP ) 
	{

		IOCPHandle = CreateIoCompletionPort((HANDLE)Listener->ListenSocket.Socket, IOCPHandle, (ULONG_PTR)Listener, 0);
		if(IOCPHandle == NULL) 
		{
			Log.Output(LG_ERR,"Failed to update listen socket to IOCP: %", sfu(GetLastError()));
			return NULL;
		}

		// Load the AcceptEx extension function from the provider for this socket
		nRet = WSAIoctl(
			Listener->ListenSocket.Socket,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&acceptex_guid,
			sizeof(acceptex_guid),
			&Listener->ListenSocket.fnAcceptEx,
			sizeof(Listener->ListenSocket.fnAcceptEx),
			&bytes,
			NULL,
			NULL
			);

		if (nRet == SOCKET_ERROR)
		{
			Log.Output(LG_ERR,"Failed to load AcceptEx: %", sfu(WSAGetLastError()));
			return NULL;
		}
	}

	SOCKET acceptSocket = CreateSocket();

	if( acceptSocket == INVALID_SOCKET) 
	{
		Log.Output(LG_ERR,"Failed to create new accept socket");
		return(FALSE);
	}

	Listener->ListenSocket.IOContextRecv.SocketAccept = acceptSocket;

	//
	// pay close attention to these parameters and buffer lengths
	//
	nRet = Listener->ListenSocket.fnAcceptEx(Listener->ListenSocket.Socket, Listener->ListenSocket.IOContextRecv.SocketAccept,
		(LPVOID)(Listener->ListenSocket.ReceiveByteBuffer),
		MAX_BUFF_SIZE - (2 * (sizeof(SOCKADDR_STORAGE) + 16)),
		sizeof(SOCKADDR_STORAGE) + 16, sizeof(SOCKADDR_STORAGE) + 16,
		&dwRecvNumBytes, 
		(LPOVERLAPPED) &(Listener->ListenSocket.IOContextRecv.Overlapped));

	if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
	{
		Log.Output(LG_ERR,"AcceptEx() failed: %", sfu(WSAGetLastError()));
		return(FALSE);
	}

	return(TRUE);

}

bool NAsynchronousWindows::Connect( NEndPoint EndPoint, NService* Service )
{
	int nRet = 0;
	DWORD dwRecvNumBytes = 0;
	DWORD bytes = 0;

	//
	// GUID to Microsoft specific extensions
	//
	GUID connectex_guid = WSAID_CONNECTEX;

	//
	//The context for listening socket uses the SockAccept member to store the
	//socket for client connection. 
	//
	// 

	SOCKET connectSocket = CreateSocket();

	if( connectSocket == INVALID_SOCKET) 
	{
		Log.Output(LG_ERR,"Failed to create new connect socket");
		return false;
	}

	sockaddr_in sinp;
	sinp.sin_addr.s_addr = INADDR_ANY;
	sinp.sin_port = 0;
	sinp.sin_family = AF_INET;

	

	int bre = bind(connectSocket,(sockaddr*)&sinp,sizeof(sinp));

	if( bre != 0 )
	{
		Log.Output(LG_ERR,"Failed to bind connect socket");
		return false;
	}


	NSocketWindows* winsck = UpdateCompletionPort(connectSocket, CIO_Connect, TRUE);

	winsck->Service = Service;
	winsck->Async = this;

	LPFN_CONNECTEX cex;

	// Load the AcceptEx extension function from the provider for this socket
	nRet = WSAIoctl(
		connectSocket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&connectex_guid,
		sizeof(connectex_guid),
		&cex,
		sizeof(cex),
		&bytes,
		NULL,
		NULL
		);

	if (nRet == SOCKET_ERROR)
	{
		Log.Output(LG_ERR,"Failed to load ConnectEx: %", sfu(WSAGetLastError()));
		return false;
	}

	// TODO: check address.Address with inet_Addr of that string.
	TString addrString = EndPoint.Address.ToString();

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr((char*)addrString.Data);
	addr.sin_port = htons(EndPoint.Port);

	//Listener->ListenSocket.IOContextRecv.SocketAccept = acceptSocket;

	winsck->IOContextSend.Operation = CIO_Connect;

	//
	// pay close attention to these parameters and buffer lengths
	//
	nRet = cex(connectSocket, (sockaddr*)&addr, sizeof(addr),
		NULL,
		NULL,
		NULL, 
		(LPOVERLAPPED) &(winsck->IOContextSend.Overlapped));


	Log.Output(LG_INF, "AcceptEx result: %", sfu(nRet));
	//if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
	if (nRet != 0)
	{
		Log.Output(LG_ERR,"AcceptEx() failed: %", sfu(WSAGetLastError()));
		return false;
	}

	return true;
}


void NAsynchronousWindows::Disconnect( NSocket* Client, bool Graceful )
{
	if( Client ) 
	{
		NSocketWindows* sock = (NSocketWindows*)Client;

		Log.Output(LG_INF,"CloseClient: Socket(%) connection closing (graceful=%)", sfu(sock->Socket), (Graceful? sfs("TRUE"): sfs("FALSE")));
		
		if( !Graceful ) 
		{

			//
			// force the subsequent closesocket to be abortative.
			//
			LINGER  lingerStruct;

			lingerStruct.l_onoff = 1;
			lingerStruct.l_linger = 0;
			setsockopt(sock->Socket, SOL_SOCKET, SO_LINGER,(char *)&lingerStruct, sizeof(lingerStruct) );
		}

		if( sock->IOContextRecv.SocketAccept != INVALID_SOCKET ) 
		{
			closesocket(sock->IOContextRecv.SocketAccept);
			sock->IOContextRecv.SocketAccept = INVALID_SOCKET;
		}

		closesocket(sock->Socket);
		sock->Socket = INVALID_SOCKET;

		sock->Service->Disconnected(sock);

		//CtxtListDeleteFrom(lpPerSocketContext);
		//lpPerSocketContext = NULL;
	} 
	else 
	{
		Log.Output(LG_INF,"CloseClient: lpPerSocketContext is NULL");
	}
}

void NAsynchronousWindows::ReceiveFrom( NSocketWindows* sock )
{
	DWORD dwRecvNumBytes;
	DWORD dwFlags = 0;
	int nRet = 0;
	WSABUF buffRecv;

	sock->IOContextRecv.Operation = CIO_Read;
	dwRecvNumBytes = 0;
	dwFlags = 0;
	buffRecv.buf = (char*)sock->ReceiveByteBuffer;
	buffRecv.len = MAX_BUFF_SIZE;

	nRet = WSARecv(sock->Socket,&buffRecv, 1,&dwRecvNumBytes,&dwFlags,&sock->IOContextRecv.Overlapped, NULL);

	if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
	{
		Log.Output(LG_ERR,"WSARecv() failed: %", sfu(WSAGetLastError()));
		Disconnect(sock, FALSE);
	}
}



void NAsynchronousWindows::Send( NSocket* Client, NPacket* Packet )
{
	DWORD dwFlags = 0;
	int nRet = 0;
	DWORD dwSendNumBytes = 0;
	
	NSocketWindows* sock = (NSocketWindows*)Client;

	sock->IOContextSend.Operation = CIO_Write;
	sock->IOContextSend.UseSendPacket(Packet);

	nRet = WSASend(
		sock->Socket,
		&sock->IOContextSend.wsabuf, 1, &dwSendNumBytes,
		dwFlags,
		&(sock->IOContextSend.Overlapped), NULL);

	if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
	{
		Log.Output(LG_ERR,"WSASend() failed: %", sfu(WSAGetLastError()));
		Disconnect(sock, FALSE);


	/*} else if( g_bVerbose ) {
		myprintf("WorkerThread %d: Socket(%d) Recv completed (%d bytes), Send posted\n", 
			GetCurrentThreadId(), lpPerSocketContext->Socket, dwIoSize);*/
	}

	//Packet->Detach();
	//delete Packet; // TODO: use a pool return here.
}

void NAsynchronousWindows::WorkerThreadFunction()
{
	BOOL bSuccess = FALSE;
	int nRet = 0;
	LPWSAOVERLAPPED lpOverlapped = NULL;
	NSocketWindows* lpPerSocketContext = NULL;
	NSocketWindows* lpAcceptSocketContext = NULL;
	NClientWindowsIO* lpIOContext = NULL; 
	//WSABUF buffRecv;
	WSABUF buffSend;
	DWORD dwRecvNumBytes = 0;
	DWORD dwSendNumBytes = 0;
	DWORD dwFlags = 0;
	DWORD dwIoSize = 0;
	//HRESULT hRet;

	ULONG_PTR Key;

	while( TRUE ) 
	{

		//
		// continually loop to service io completion packets
		//
		bSuccess = GetQueuedCompletionStatus(IOCPHandle,&dwIoSize,&Key,(LPOVERLAPPED *)&lpOverlapped,INFINITE );
		if( !bSuccess )
		{
			Log.Output(LG_ERR,"GetQueuedCompletionStatus() failed: %",sfu(GetLastError()));
		}

		if( Key == NULL ) 
		{
			return;
		}

		if( EndServer ) 
		{
			return;
		}

		lpIOContext = (NClientWindowsIO*)((ULONG_PTR)lpOverlapped - ((ULONG_PTR)&lpIOContext->Overlapped - (ULONG_PTR)lpIOContext));

		if( lpIOContext->Operation != CIO_Accept ) 
		{
			lpPerSocketContext = (NSocketWindows*)Key;

			if ( lpIOContext->Operation != CIO_Connect )
			{
				if( !bSuccess || (bSuccess && (0 == dwIoSize)) ) 
				{
					Disconnect(lpPerSocketContext, FALSE); 
					continue;
				}
			}
			else
			{
				if ( !bSuccess )
				{
					Log.Output(LG_INF,"WorkerThread %: Socket(%) ConnectEx failed: %",sfu(GetCurrentThreadId()),sfu(lpPerSocketContext->Socket),sfu(WSAGetLastError()));
					Disconnect(lpPerSocketContext, FALSE);
					continue;
				}
			}
		}

		//
		// determine what type of IO packet has completed by checking the PER_IO_CONTEXT 
		// associated with this socket.  This will determine what action to take.
		//
		switch( lpIOContext->Operation ) 
		{
		case  CIO_Connect:
			{
				lpPerSocketContext->Connected();
				Log.Output(LG_INF,"WorkerThread %: Socket(%) ConnectEx completed (% bytes)",sfu(GetCurrentThreadId()),sfu(lpPerSocketContext->Socket),sfu(dwIoSize));
				ReceiveFrom(lpPerSocketContext);
			}
			break;

		case CIO_Accept:
			{
				NListenerWindows* Listener = (NListenerWindows*)Key;
				lpPerSocketContext = &Listener->ListenSocket;

				nRet = setsockopt(lpPerSocketContext->IOContextRecv.SocketAccept, SOL_SOCKET,SO_UPDATE_ACCEPT_CONTEXT,(char *)&Listener->ListenSocket.Socket,sizeof(Listener->ListenSocket.Socket));

				if( nRet == SOCKET_ERROR ) 
				{
					Log.Output(LG_ERR,"setsockopt(SO_UPDATE_ACCEPT_CONTEXT) failed to update accept socket");
					WSASetEvent(CleanupEvent[0]);
					return;
				}

				lpAcceptSocketContext = UpdateCompletionPort(lpPerSocketContext->IOContextRecv.SocketAccept, CIO_Accept, TRUE);
				lpAcceptSocketContext->Service = Listener->Service;
				lpAcceptSocketContext->Async = Listener->Service->Async;
				
				//lpAcceptSocketContext->IOContextRecv.UseRecvPacket( Listener->Service->CreateReceiveBuffer() );

				if( lpAcceptSocketContext == NULL ) 
				{
					Log.Output(LG_ERR,"failed to update accept socket to IOCP");
					WSASetEvent(CleanupEvent[0]);
					return;
				}

				lpAcceptSocketContext->Connected();

				if( dwIoSize ) 
				{
					lpAcceptSocketContext->IOContextRecv.Operation = CIO_Read;
					lpAcceptSocketContext->IOContextRecv.TotalBytes = dwIoSize;
					lpAcceptSocketContext->IOContextRecv.SentBytes = 0;
					lpAcceptSocketContext->IOContextRecv.wsabuf.len = dwIoSize;
					lpAcceptSocketContext->IOContextRecv.CurrentPacket->Length = dwIoSize;

					MemoryDriver::Copy(lpAcceptSocketContext->ReceiveByteBuffer,lpPerSocketContext->ReceiveByteBuffer,dwIoSize);


					lpAcceptSocketContext->Received(lpAcceptSocketContext->IOContextRecv.CurrentPacket);

					Log.Output(LG_INF,"WorkerThread %: Socket(%) AcceptEx completed (% bytes)",sfu(GetCurrentThreadId()),sfu(lpPerSocketContext->Socket),sfu(dwIoSize));
				} 
				else 
				{
					ReceiveFrom(lpAcceptSocketContext);
					//
					// AcceptEx completes but doesn't read any data so we need to post
					// an outstanding overlapped read.
					//
					/*lpAcceptSocketContext->IOContextRecv.Operation = CIO_Read;
					dwRecvNumBytes = 0;
					dwFlags = 0;
					buffRecv.buf = lpAcceptSocketContext->IOContextRecv.Buffer;
					buffRecv.len = MAX_BUFF_SIZE;

					nRet = WSARecv(lpAcceptSocketContext->Socket,&buffRecv, 1,&dwRecvNumBytes,&dwFlags,&lpAcceptSocketContext->IOContext.Overlapped, NULL);

					if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
					{
						Log.Output(LG_ERR,"WSARecv() failed: %", sfu(WSAGetLastError()));
						CloseClient(lpAcceptSocketContext, FALSE);
					}*/
				}

				//
				//Time to post another outstanding AcceptEx
				//
				if( !CreateAcceptSocket(Listener,false) ) 
				{
					Log.Output(LG_ERR,"Please shut down and reboot the server.");
					WSASetEvent(CleanupEvent[0]);
					return;
				}
			}
			break;


		case CIO_Read:
			lpIOContext->TotalBytes = dwIoSize;
			lpIOContext->SentBytes = 0;
			lpIOContext->wsabuf.len = dwIoSize;
			lpIOContext->CurrentPacket->Length = dwIoSize;
			lpPerSocketContext->Received(lpIOContext->CurrentPacket);
			Log.Output(LG_INF,"WorkerThread %: Socket(%) Recv completed (% bytes)",  sfu(GetCurrentThreadId()), sfu(lpPerSocketContext->Socket), sfu(dwIoSize));
			break;

		case CIO_Write:

			//lpPerSocketContext->Service->Sent(lpPerSocketContext,lpIOContext);

			//
			// a write operation has completed, determine if all the data intended to be
			// sent actually was sent.
			//
			lpIOContext->Operation = CIO_Write;
			lpIOContext->SentBytes  += dwIoSize;
			dwFlags = 0;
			if( lpIOContext->SentBytes < lpIOContext->TotalBytes ) 
			{

				//
				// the previous write operation didn't send all the data,
				// post another send to complete the operation
				//
				buffSend.buf = (char*)lpIOContext->CurrentPacket->Data + lpIOContext->SentBytes;
				buffSend.len = lpIOContext->TotalBytes - lpIOContext->SentBytes;
				nRet = WSASend (
					lpPerSocketContext->Socket,
					&buffSend, 1, &dwSendNumBytes,
					dwFlags,
					&(lpIOContext->Overlapped), NULL);
				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
				{
					Log.Output(LG_ERR,"WSASend() failed: %", sfu(WSAGetLastError()));
					Disconnect(lpPerSocketContext, FALSE);
				} 
				else 
				{
					Log.Output(LG_INF,"WorkerThread %: Socket(%) Send partially completed (% bytes)", sfu(GetCurrentThreadId()), sfu(lpPerSocketContext->Socket), sfu(dwIoSize));
				}
			} 
			else 
			{
				lpPerSocketContext->Sent(lpIOContext->CurrentPacket);
				Log.Output(LG_INF,"WorkerThread %: Socket(%) Send completed (% bytes)", sfu(GetCurrentThreadId()), sfu(lpPerSocketContext->Socket), sfu(dwIoSize));

				//
				// previous write operation completed for this socket, post another recv
				//
				/*lpIOContext->Operation = CIO_Read; 
				dwRecvNumBytes = 0;
				dwFlags = 0;
				buffRecv.buf = lpIOContext->Buffer;
				buffRecv.len = MAX_BUFF_SIZE;

				nRet = WSARecv(
					lpPerSocketContext->Socket,
					&buffRecv, 1, &dwRecvNumBytes,
					&dwFlags,
					&lpIOContext->Overlapped, NULL);

				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
				{
					Log.Output(LG_ERR,"WSARecv() failed: %", sfu(WSAGetLastError()));
					CloseClient(lpPerSocketContext, FALSE);
				} 
				else 
				{
					Log.Output(LG_INF,"WorkerThread %: Socket(%) Send completed (% bytes), Recv posted", sfu(GetCurrentThreadId()), sfu(lpPerSocketContext->Socket), sfu(dwIoSize));
				}*/
			}
			break;
		} //switch
	} //while
}

SOCKET NAsynchronousWindows::CreateSocket()
{
	int nRet = 0;
	int nZero = 0;
	SOCKET sdSocket = INVALID_SOCKET;

	sdSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED); 
	if( sdSocket == INVALID_SOCKET ) 
	{
		Log.Output(LG_ERR,"WSASocket(sdSocket) failed: %", sfi(WSAGetLastError()));
		return(sdSocket);
	}

	nZero = 0;
	nRet = setsockopt(sdSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	if( nRet == SOCKET_ERROR) 
	{
		Log.Output(LG_ERR,"setsockopt(SNDBUF) failed: %", sfi(WSAGetLastError()));
		return(sdSocket);
	}

	return(sdSocket);
}


/*void Main()
{
	NServerWindows TheServer;
	
	NServiceHTTP HttpService;

	TheServer.AddService(&HttpService);

	HttpService.Configuration.Add()

	TheServer.CreateListener("192.168.2.2",80, NP_TCP, &HttpService);
	TheServer.CreateListener("127.0.0.1",31, NP_TCP, &HttpService);

	TheServer.StartServer();

}*/

#include "twintools.h"

bool NAsynchronousWindows::CreateListenerTask( NIPAddress Device, ui16 Port, NProtocol Protocol, NService* Service )
{
	NListenerWindows* WinListener = new NListenerWindows();

	WinListener->Device = Device;
	WinListener->Port = Port;
	WinListener->Protocol = Protocol;
	WinListener->Service = Service;

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

	TString portstr = TConvert::ToString(Port);
	TString addr = Device.ToString();

	if( getaddrinfo((char*)addr.Data, (char*)portstr.Data, &hints, &addrlocal) != 0 ) 
	{
		Log.Output(LG_ERR,"getaddrinfo() failed with error %", sfs(TWinTools::ErrorToStringWithCode(WSAGetLastError())));
		return(FALSE);
	}

	if( addrlocal == NULL ) 
	{
		Log.Output(LG_ERR,"getaddrinfo() failed to resolve/convert the interface");
		return(FALSE);
	}

	WinListener->ListenSocket.Socket = CreateSocket();
	if( WinListener->ListenSocket.Socket == INVALID_SOCKET) 
	{
		freeaddrinfo(addrlocal);
		return(FALSE);
	}

	nRet = bind(WinListener->ListenSocket.Socket, addrlocal->ai_addr, (int) addrlocal->ai_addrlen);
	if( nRet == SOCKET_ERROR) 
	{
		Log.Output(LG_ERR,"bind() failed: %", sfi(WSAGetLastError()));
		freeaddrinfo(addrlocal);
		return(FALSE);
	}

	nRet = listen(WinListener->ListenSocket.Socket, 5);
	if( nRet == SOCKET_ERROR ) 
	{
		Log.Output(LG_ERR,"listen() failed: %", sfi(WSAGetLastError()));
		freeaddrinfo(addrlocal);
		return(FALSE);
	}

	freeaddrinfo(addrlocal);

	Listeners.Add(WinListener);

	CreateAcceptSocket(WinListener,true);

	return true;
}

void NAsynchronousWindows::StopServer()
{
	EndServer = true;
}

void NAsynchronousWindows::RestartServer()
{
	Restart = true;
}

void NAsynchronousWindows::CreateListener( NIPAddress Device, ui16 Port, NProtocol Protocol, NService* Service )
{
	NServerOperation* nsp = new NServerOperation();
	
	nsp->Operation = NServerOperation::OT_CREATELISTENER;

	CreateListenerParameters* clp = (CreateListenerParameters*)nsp->Parameter;
	clp->Device = Device;
	clp->Port = Port;
	clp->Protocol = Protocol;
	clp->Service = Service;

	TaskQueue.Push(nsp);
}


#endif
