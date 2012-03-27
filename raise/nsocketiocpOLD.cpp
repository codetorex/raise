#include "stdafx.h"

#if 0

bool TIOServiceIOCP::CreateAcceptSocket( NListenerWindows* Listener, bool UpdateIOCP )
{
	int nRet = 0;
	DWORD dwRecvNumBytes = 0;
	DWORD bytes = 0;

	//
	// GUID to Microsoft specific extensions
	//
	

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

bool TIOServiceIOCP::Connect( NEndPoint EndPoint, NService* Service )
{
	int nRet = 0;
	DWORD dwRecvNumBytes = 0;
	DWORD bytes = 0;

	//
	// GUID to Microsoft specific extensions
	//

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


	NSocketIOCP* winsck = UpdateCompletionPort(connectSocket, CIO_Connect, TRUE);

	winsck->Service = Service;
	winsck->Async = this;


	// Load the AcceptEx extension function from the provider for this socket


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


void TIOServiceIOCP::Disconnect( NSocket* Client, bool Graceful )
{
	if( Client ) 
	{
		NSocketIOCP* sock = (NSocketIOCP*)Client;

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

void TIOServiceIOCP::ReceiveFrom( NSocketIOCP* sock )
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



void TIOServiceIOCP::Send( NSocket* Client, NPacket* Packet )
{
	DWORD dwFlags = 0;
	int nRet = 0;
	DWORD dwSendNumBytes = 0;
	
	NSocketIOCP* sock = (NSocketIOCP*)Client;

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

void TIOServiceIOCP::WorkerThreadFunction()
{
	BOOL bSuccess = FALSE;
	int nRet = 0;
	LPWSAOVERLAPPED lpOverlapped = NULL;
	NSocketIOCP* lpPerSocketContext = NULL;
	NSocketIOCP* lpAcceptSocketContext = NULL;
	NSocketIOCPOperation* lpIOContext = NULL; 
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

		lpIOContext = (NSocketIOCPOperation*)((ULONG_PTR)lpOverlapped - ((ULONG_PTR)&lpIOContext->Overlapped - (ULONG_PTR)lpIOContext));

		if( lpIOContext->Operation != CIO_Accept ) 
		{
			lpPerSocketContext = (NSocketIOCP*)Key;

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

#endif