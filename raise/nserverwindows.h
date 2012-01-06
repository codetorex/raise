#ifndef NSERVERWINDOWS_H
#define NSERVERWINDOWS_H

#include "nserver.h"
#include "tarray.h"
#include "tthread.h"
#include "tcriticalsection.h"
#include "tbytearray.h"
#include "tlinkedlist.h"
#include "tstack.h"


// Bu sekilde olunca bridge pattern oluyo


/**
 * Damned player :P
 */
class DPlayer
{
public:
	NSocket* Client;


};


enum PacketIO
{
	CIO_Accept,
	CIO_Read,
	CIO_Write,
};

class NClientWindowsIO
{
public:
	NClientWindowsIO( SOCKET sd = 0, PacketIO op = CIO_Accept)
	{
		Overlapped.Internal = 0;
		Overlapped.InternalHigh = 0;
		Overlapped.Offset = 0;
		Overlapped.OffsetHigh = 0;
		Overlapped.hEvent = NULL;

		Operation = op;
		TotalBytes = 0;
		SentBytes = 0;
		/*wsabuf.buf = Buffer;
		wsabuf.len = MAX_BUFF_SIZE;*/
		SocketAccept = INVALID_SOCKET;

		Working = false;
		CurrentPacket = 0;
	}

	PacketIO			Operation;

	WSAOVERLAPPED		Overlapped;

	WSABUF				wsabuf;
	SOCKET				SocketAccept;
	NPacket*			CurrentPacket;
	ui32				SentBytes;
	ui32				TotalBytes;

	bool				Working;

	inline void UseRecvPacket(NPacket* pck)
	{
		wsabuf.buf = (char*)pck->Data;
		wsabuf.len = pck->Capacity;
		CurrentPacket = pck;
		TotalBytes = pck->Length;
	}

	inline void UseSendPacket(NPacket* pck)
	{
		wsabuf.buf = (char*)pck->Data;
		wsabuf.len = pck->Length;
		CurrentPacket = pck;
		TotalBytes = pck->Length;
		SentBytes = 0;
	}
};

class NSocketWindows: public NSocket
{
public:
	NSocketWindows();

	SOCKET			Socket;
	LPFN_ACCEPTEX	fnAcceptEx;

	NClientWindowsIO IOContextRecv; // also accept context too
	NClientWindowsIO IOContextSend; // hmm allows duplex connection?
};

class NListenerWindows: public NListener
{
public:

	NSocketWindows ListenSocket;

	void CreateAcceptSocket();
};



// NCLIENT FACTORY?

class NServerOperation
{
public:
	enum OperationType
	{
		OT_CREATELISTENER,
	};

	OperationType	Operation;
	byte			Parameter[256];
};

struct CreateListenerParameters
{
	NIPAddress	Device;
	ui16		Port;
	NProtocol	Protocol; 
	NService*	Service;
};

/**
 * IOCP based scalable windows server.
 * Everything needs to be scalable nowadays, who needs old techniques anyway?
 */
class NServerWindows: public NServer
{
private:
	WSAEVENT CleanupEvent[1];
	void Cleanup();

	HANDLE IOCPHandle;
	TCriticalSection ContextListCS;		// guard access to the global context list

	bool Restart;
	bool EndServer;
	int	BestWorkerCount;

	bool CreateListenSocket();

	bool CreateListenSocket(NListener* acceptor);

	//bool CreateAcceptSocket(bool updateIOCP);

	SOCKET CreateSocket();

	TArray<NListenerWindows*> Listeners;

	NSocketWindows* AllocateNewSocket(SOCKET sd, PacketIO op);

	NSocketWindows* UpdateCompletionPort(SOCKET sd, PacketIO op, bool addtoList);

	bool CreateAcceptSocket(NListenerWindows* Listener, bool UpdateIOCP);

	void ReceiveFrom(NSocketWindows* sock);


	/// This stack holds the tasks need to be done after server started
	TStack<NServerOperation*> TaskQueue;

	bool CreateListenerTask( NIPAddress Device, ui16 Port, NProtocol Protocol, NService* Service);

public:

	TArray< TThread* >	WorkerThreads;

	TThread*			MainThread;

	void AddService( NService* service )
	{
		Services.Add(service); // hmm
		service->Server = this;
	}

	void Initialize();

	void StartServer();
	void StopServer();
	void RestartServer();

	void MainThreadFunction();
	void WorkerThreadFunction();


	void Disconnect( NSocket* Client, bool Graceful = false);
	void Send( NSocket* Client, NPacket* Packet);

	void CreateListener( NIPAddress Device, ui16 Port, NProtocol Protocol, NService* Service);
};

#endif
