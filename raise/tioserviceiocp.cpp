#include "stdafx.h"
#include "texception.h"
#include "tlog.h"

#ifdef WIN32
#include "tplatform.h"
#include "tioserviceiocp.h"

bool TIOServiceIOCP::Initialized = false;

void TIOServiceIOCP::InitializeWinsock()
{
	if (Initialized)
		return;

	WSADATA wsaData;

	int nRet = WSAStartup(0x202, &wsaData);
	if( nRet != 0 ) 
	{
		throw Exception("WSAStartup() failed: %", sfi(nRet));
	}

	Initialized = true;
}

void TIOServiceIOCP::Start( ui32 workerCount )
{
	ChangeStatus(Starting);

	/// INITIALIZE WINSOCK
	InitializeWinsock();

	/// LOAD ACCEPTEX AND CONNECTEX EXTENSIONS
	int nRet;
	DWORD bytes = 0;
	SOCKET tmpSocket = socket(A_InterNetworkV4,S_Stream,P_TCP);

	GUID acceptex_guid = WSAID_ACCEPTEX;
	nRet = WSAIoctl(
		tmpSocket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&acceptex_guid,
		sizeof(acceptex_guid),
		&pAcceptEx,
		sizeof(pAcceptEx),
		&bytes,
		NULL,
		NULL
		);

	if (nRet == SOCKET_ERROR)
	{
		throw("Failed to load AcceptEx: %", sfs(Platform.GetErrorDescription(WSAGetLastError())));
	}

	bytes = 0;
	GUID connectex_guid = WSAID_CONNECTEX;
	nRet = WSAIoctl(
		tmpSocket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&connectex_guid,
		sizeof(connectex_guid),
		&pConnectEx,
		sizeof(pConnectEx),
		&bytes,
		NULL,
		NULL
		);

	if (nRet == SOCKET_ERROR)
	{
		throw("Failed to load ConnectEx: %", sfs(Platform.GetErrorDescription(WSAGetLastError())));
	}

	closesocket(tmpSocket);

	/// GET WORKER THREAD COUNT
	if (workerCount == 0)
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		workerCount = systemInfo.dwNumberOfProcessors * 2;
	}

	/// CREATE IOCP
	IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if( IOCPHandle == NULL ) 
	{
		throw Exception("CreateIoCompletionPort() failed to create I/O completion port: %", sfu(GetLastError()));
	}

	Running = true;

	/// CREATE WORKERS
	for( ui32 i=0; i< workerCount; i++ ) 
	{
		TThread* curWorker = new TThread(GetHandler(this,&TIOServiceIOCP::WorkerMain));
		curWorker->Start();
		Workers.Add(curWorker);
	}

	ChangeStatus(Started);
}

void TIOServiceIOCP::Start()
{
	Start(0);
}


void TIOServiceIOCP::WorkerMain()
{
	BOOL CmpStatus;

	DWORD IOSize = 0;
	ULONG_PTR Key;
	LPWSAOVERLAPPED Overlapped = NULL;

	NSocketIOCPOperation* Operation = 0;
	NSocketIOCP* CurrentSocket;
	DWORD dwSendNumBytes = 0;

	int nRet;

	SystemError tmpError;

	while (1)
	{
		CmpStatus = GetQueuedCompletionStatus(IOCPHandle,&IOSize,&Key,(LPOVERLAPPED *)&Overlapped,INFINITE );

		if (!CmpStatus)
		{
			tmpError.SetErrorMessage(GetLastError());
			Log.Output(LG_ERR,"GetQueuedCompletionStatus() failed: %",sfs(tmpError.Message));
		}
		else
		{
			tmpError.Unset();
		}

		if (Key == NULL)
		{
			break;
		}

		if (!Running)
		{
			break;
		}

		CurrentSocket = (NSocketIOCP*)Key;
		Operation = (NSocketIOCPOperation*)((ULONG_PTR)Overlapped - ((ULONG_PTR)&Operation->Overlapped - (ULONG_PTR)Operation));
		Operation->Working = false;

		switch(Operation->Operation)
		{
		case CIO_Connect:
			{
				if (!tmpError)
				{
					Log.Output(LG_INF,"WorkerThread %: Socket(%) ConnectEx completed (% bytes)",sfu(GetCurrentThreadId()),sfu(CurrentSocket->Socket),sfu(IOSize));
				}
				else
				{
					Log.Output(LG_ERR,"WorkerThread %: Socket(%) ConnectEx failed %",sfu(GetCurrentThreadId()),sfu(CurrentSocket->Socket),sfs(tmpError.Message));
				}
				Operation->ConnectCb->call( tmpError );
				
			}
			break;

		case CIO_Accept:
			{
				if (!tmpError)
				{
					nRet = setsockopt(CurrentSocket->IOContextRecv.AcceptSocket->Socket, SOL_SOCKET,SO_UPDATE_ACCEPT_CONTEXT,(char *)&CurrentSocket->Socket,sizeof(CurrentSocket->Socket));

					if( nRet == SOCKET_ERROR ) 
					{
						tmpError.SetErrorMessage(WSAGetLastError());
						Log.Output(LG_ERR,"setsockopt(SO_UPDATE_ACCEPT_CONTEXT) failed to update accept socket");
					}
					else
					{
						NSocketIOCP* acceptedSocket = (NSocketIOCP*)CurrentSocket->IOContextRecv.AcceptSocket;
						IntroduceIOCP(acceptedSocket);
					}
				}
				Operation->AcceptCb->call(  CurrentSocket->IOContextRecv.AcceptObject,tmpError);
			}
			break;

		case CIO_Read:
			{
				if (IOSize == 0)
				{
					tmpError.SetErrorMessage(WSAECONNRESET);
					Log.Output(LG_ERR,"WSARecv() thinks connection is closed %", sfs(tmpError.Message));
				}
				Operation->Working = false;
				Operation->CurrentPacket->Length = IOSize;
				Operation->RecvCb->call( tmpError, IOSize);
			}
			break;

		case CIO_Write:
			{
				Operation->Working = false;
				Operation->SentBytes += IOSize;

				if (!tmpError)
				{
					if (IOSize == 0)
					{
						tmpError.SetErrorMessage(WSAECONNRESET);
					}
					else
					{
						if (Operation->SentBytes < Operation->TotalBytes)
						{
							CurrentSocket->IOContextSend.wsabuf.buf += IOSize;
							CurrentSocket->IOContextSend.wsabuf.len -= IOSize;
							nRet = WSASend(CurrentSocket->Socket, &CurrentSocket->IOContextSend.wsabuf,1,&dwSendNumBytes,0,&(CurrentSocket->IOContextSend.Overlapped),NULL);

							if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
							{
								tmpError.SetErrorMessage(WSAGetLastError());
								Log.Output(LG_ERR,"WSASend() failed: %", sfs(tmpError.Message));
							}
						}
					}
				}

				
				Operation->SendCb->call(tmpError);
			}
			break;
		}
	}
}

void TIOServiceIOCP::Stop()
{
	ChangeStatus(Stopping);
	Running = false;

	// Cause worker threads to exit
	if( IOCPHandle ) 
	{
		for( ui32 i = 0; i < Workers.Count; i++ )
		{
			PostQueuedCompletionStatus(IOCPHandle, 0, 0, NULL);
		}
	}

	HANDLE tHandles[64];
	for (ui32 i=0;i<Workers.Count;i++)
	{
		tHandles[i] = Workers.Item[i]->ThreadHandle;
	}

	if( WAIT_OBJECT_0 != WaitForMultipleObjects(Workers.Count,  tHandles, TRUE, 1000) )
	{
		Log.Output(LG_ERR,"WaitForMultipleObjects() failed: %", sfu(GetLastError()));
	}
	else
	{
		for( ui32 i=0; i< Workers.Count ; i++ ) 
		{
			TThread* curThread = Workers.Item[i];
			if( tHandles[i] != INVALID_HANDLE_VALUE )
			{
				CloseHandle(tHandles[i]);

			}
			curThread->Abort();
			curThread->ThreadHandle = INVALID_HANDLE_VALUE;
			tHandles[i] = INVALID_HANDLE_VALUE;
		}
	}

	if( IOCPHandle ) 
	{
		CloseHandle(IOCPHandle);
		IOCPHandle = NULL;
	}

	ChangeStatus(Stopped);
}


NSocket* TIOServiceIOCP::CreateSocket(AddressFamilies _AddressFamily, SocketTypes _SocketType, ProtocolTypes _ProtocolType)
{
	int nRet = 0;
	int nZero = 0;
	SOCKET sdSocket = INVALID_SOCKET;

	sdSocket = WSASocket(_AddressFamily, _SocketType, _ProtocolType, NULL, 0, WSA_FLAG_OVERLAPPED); 
	if( sdSocket == INVALID_SOCKET ) 
	{
		Log.Output(LG_ERR,"WSASocket(sdSocket) failed: %", sfs(Platform.GetErrorDescription(WSAGetLastError())));
		return NULL;
	}

	nZero = 0;
	nRet = setsockopt(sdSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	if( nRet == SOCKET_ERROR) 
	{
		closesocket(sdSocket);
		Log.Output(LG_ERR,"setsockopt(SNDBUF) failed: %", sfs(Platform.GetErrorDescription(WSAGetLastError())));
		return NULL;
	}

	NSocketIOCP* newSocket = new NSocketIOCP(_AddressFamily,_SocketType,_ProtocolType,sdSocket, this);
	
	return newSocket;
}

/**
 * Introduces given socket to IOCP. After this function called operations will be handled by IOCP.
 */
void TIOServiceIOCP::IntroduceIOCP( NSocketIOCP* sck )
{
	if (sck->Introduced)
	{
		throw Exception("This should not happened");
	}

	IOCPHandle = CreateIoCompletionPort((HANDLE)sck->Socket, IOCPHandle, (ULONG_PTR)sck, 0);
	if(IOCPHandle == NULL) 
	{
		Log.Output(LG_ERR,"CreateIoCompletionPort() failed: %", sfs(Platform.GetErrorDescription(GetLastError())));
		delete sck;
		return;
	}

	sck->Introduced = true;

	Log.Output(LG_INF,"UpdateCompletionPort: Socket(%) added to IOCP", sfu(sck->Socket));
}




void TIOServiceIOCP::Restart()
{
	int oldWorkerCount = Workers.Count;
	Stop();
	Start(oldWorkerCount);
}


void TIOServiceIOCP::AcceptAsync( NSocket* sck, NSocket* target, void* object, NPacket* first, AcceptedCallback* Callback )
{
	NSocketIOCP* listener = (NSocketIOCP*)sck;
	
	if (listener->IOContextRecv.Working)
	{
		throw Exception("Socket operation is already in working state");
	}

	listener->IOContextRecv.Operation = CIO_Accept;
	listener->IOContextRecv.AcceptCb = Callback;
	listener->IOContextRecv.UseRecvPacket(first);
	listener->IOContextRecv.Working = true;
	listener->IOContextRecv.AcceptObject = object;

	if (!listener->Introduced)
	{
		IntroduceIOCP( listener );
	}

	listener->IOContextRecv.AcceptSocket = target;
	
	DWORD dwRecvNumBytes = 0;

	/*int nRet = pAcceptEx(sck->Socket, target->Socket,first->Data,
		first->Capacity - (2 * (sizeof(SOCKADDR_STORAGE)+16)),
		sizeof(SOCKADDR_STORAGE) + 16, sizeof(SOCKADDR_STORAGE) + 16,
		&dwRecvNumBytes, 
		(LPOVERLAPPED) &(listener->IOContextRecv.Overlapped));*/

	int nRet = pAcceptEx(sck->Socket, target->Socket,first->Data,
		0,
		sizeof(SOCKADDR_STORAGE) + 16, sizeof(SOCKADDR_STORAGE) + 16,
		&dwRecvNumBytes, 
		(LPOVERLAPPED) &(listener->IOContextRecv.Overlapped));

	if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
	{
		Log.Output(LG_ERR,"AcceptEx() failed: %", sfs(Platform.GetErrorDescription(WSAGetLastError())));
	}
}

void TIOServiceIOCP::ConnectAsync( NSocket* sck, NEndPoint& endpoint, ConnectCallback* Callback )
{
	NSocketIOCP* isck = (NSocketIOCP*)sck;

	NEndPoint any;
	sck->Bind(any);

	IntroduceIOCP( isck );

	isck->IOContextSend.Operation = CIO_Connect;
	isck->IOContextSend.ConnectCb = Callback;
	isck->IOContextSend.Working = true;


	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = endpoint.Address.Address;
	addr.sin_port = htons(endpoint.Port);

	int nRet = pConnectEx( isck->Socket,  (sockaddr*)&addr, sizeof(addr), 0,0,0, (LPOVERLAPPED) &(isck->IOContextSend.Overlapped));

	if (nRet != 0)
	{
		Log.Output(LG_ERR,"AcceptEx() failed: %", sfs(Platform.GetErrorDescription(WSAGetLastError())));
	}
}

void TIOServiceIOCP::SendAsync( NSocket* sck, NPacket* packet , SendCallback* Callback )
{
	NSocketIOCP* isock = (NSocketIOCP*)sck;

	if (isock->IOContextSend.Working)
	{
		Log.Output(LG_ERR,"IOCP still sending data!");
		return;
	}

	isock->IOContextSend.Operation = CIO_Write;
	isock->IOContextSend.UseSendPacket(packet);
	isock->IOContextSend.Working = true;
	isock->IOContextSend.SendCb = Callback;
	isock->IOContextSend.TotalBytes = packet->Length;
	isock->IOContextSend.SentBytes = 0;

	DWORD dwFlags = 0;
	int nRet = 0;
	DWORD dwSendNumBytes = 0;

	nRet = WSASend(isock->Socket, &isock->IOContextSend.wsabuf,1,&dwSendNumBytes,dwFlags,&(isock->IOContextSend.Overlapped),NULL);

	if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
	{
		SystemError err;
		err.SetErrorMessage(WSAGetLastError());
		Log.Output(LG_ERR,"WSASend() failed: %",  sfs(err.Message));
		Callback->call(err);
	}
}

void TIOServiceIOCP::RecvAsync( NSocket* sck, NPacket* packet, ReceiveCallback* Callback )
{
	NSocketIOCP* isock = (NSocketIOCP*)sck;

	if (isock->IOContextRecv.Working)
	{
		Log.Output(LG_ERR,"IOCP still receiving data!");
		return;
	}

	isock->IOContextRecv.Operation = CIO_Read;
	isock->IOContextRecv.UseRecvPacket(packet);
	isock->IOContextRecv.Working = true;
	isock->IOContextRecv.RecvCb = Callback;

	DWORD dwRecvNumBytes;
	DWORD dwFlags = 0;
	int nRet = 0;

	nRet = WSARecv(isock->Socket,&(isock->IOContextRecv.wsabuf), 1,&dwRecvNumBytes,&dwFlags,&(isock->IOContextRecv.Overlapped), NULL);

	if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
	{
		SystemError err;
		err.SetErrorMessage(WSAGetLastError());
		Log.Output(LG_ERR,"WSARecv() failed: %", sfs(err.Message));
		Callback->call(err,0);
	}
}

void TIOServiceIOCP::Disconnect( NSocket* Client, bool Graceful /*= false*/ )
{
	NSocketIOCP* isock = (NSocketIOCP*)Client;

	Log.Output(LG_INF,"CloseClient: Socket(%) connection closing (graceful=%)", sfu(isock->Socket), (Graceful? sfs("TRUE"): sfs("FALSE")));

	if( !Graceful ) 
	{
		LINGER  lingerStruct;

		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0;
		setsockopt(isock->Socket, SOL_SOCKET, SO_LINGER,(char *)&lingerStruct, sizeof(lingerStruct) );
	}

	if( isock->IOContextRecv.AcceptSocket != 0 ) 
	{
		Log.Output(LG_INF,"Trololol!");
		
	}

	Client->Disconnect();
	// I HOPE IT WILL READ 0  WHEN DISCONNECTED? IF NOT THE CASE WE SHOULD DO IT HERE

	delete Client; // HMM
}

#endif


