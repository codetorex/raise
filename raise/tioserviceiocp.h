#ifndef NSERVERWINDOWS_H
#define NSERVERWINDOWS_H

#ifdef WIN32


#include "tioservice.h"
#include "tarray.h"
#include "tthread.h"
#include "tcriticalsection.h"
#include "tbytearray.h"
#include "tlinkedlist.h"
#include "tstack.h"
#include "nsocket.h"



// Bu sekilde olunca bridge pattern oluyo

enum PacketIO
{
	CIO_Accept,
	CIO_Read,
	CIO_Write,
	CIO_Connect,
};

class NSocketIOCPOperation
{
public:
	NSocketIOCPOperation( PacketIO op = CIO_Accept)
	{
		Overlapped.Internal = 0;
		Overlapped.InternalHigh = 0;
		Overlapped.Offset = 0;
		Overlapped.OffsetHigh = 0;
		Overlapped.hEvent = NULL;

		Operation = op;
		TotalBytes = 0;
		SentBytes = 0;
		Working = false;
		CurrentPacket = 0;
		AcceptSocket= 0;
	}

	PacketIO			Operation;
	WSAOVERLAPPED		Overlapped;
	WSABUF				wsabuf;
	NPacket*			CurrentPacket;
	NSocket*			AcceptSocket;
	ui32				SentBytes;
	ui32				TotalBytes;
	bool				Working;
	void*				AcceptObject; // TODO: make a different delegate thing, that can take pre stored parameters? 

	union
	{
		ConnectCallback*	ConnectCb;
		AcceptedCallback*	AcceptCb;
		SendCallback*		SendCb;
		ReceiveCallback*	RecvCb;
	};

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

/**
 * Berkeley sockets derived socket implementation for windows that supports asynchronous I/O.
 * TODO: make these guys not derived.
 */
class NSocketIOCP: public NSocket
{
public:


	NSocketIOCP(AddressFamilies _AddressFamily, SocketTypes _SocketType, ProtocolTypes _ProtocolType, SOCKET _Socket, TIOService* svc): NSocket(_AddressFamily, _SocketType, _ProtocolType, _Socket)
	{
		Service = svc;
		Introduced = false;
	}

	bool Introduced;

	NSocketIOCPOperation IOContextRecv; // also accept context too
	NSocketIOCPOperation IOContextSend; // hmm allows duplex connection?
};

/**
 * IOCP based scalable windows server.
 * Everything needs to be scalable nowadays, who needs old techniques anyway?
 */
class TIOServiceIOCP: public TIOService
{
private:
	HANDLE IOCPHandle;

	LPFN_CONNECTEX pConnectEx;
	LPFN_ACCEPTEX pAcceptEx;

	void IntroduceIOCP( NSocketIOCP* sck );

	static bool Initialized;

	static void InitializeWinsock();

public:

	TIOServiceIOCP()
	{
		pAcceptEx = 0;
		pConnectEx = 0;
	}

	void Start(ui32 workerCount);

	void Start();

	void Stop();
	void Restart();

	void WorkerMain();

	NSocket* CreateSocket(AddressFamilies _AddressFamily, SocketTypes _SocketType, ProtocolTypes _ProtocolType);

	void Disconnect( NSocket* Client, bool Graceful = false);

	void AcceptAsync ( NSocket* sck, NSocket* target, void* object, NPacket* first, AcceptedCallback* Callback );

	void ConnectAsync ( NSocket* sck, NEndPoint& endpoint, ConnectCallback* Callback );

	void SendAsync ( NSocket* sck, NPacket* packet , SendCallback* Callback);

	void RecvAsync ( NSocket* sck, NPacket* packet, ReceiveCallback* Callback );
};

#endif

#endif
