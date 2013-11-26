#ifndef TIOSERVICE_H
#define TIOSERVICE_H

#include "naddress.h"
#include "npacket.h"
#include "tarray.h"
#include "tevent.h"
#include "nsocketdefs.h"
#include "nsocket.h"

class TIOService;
class NService;
class TThread;
class NSocketAsync;

class TIOStatus
{
public:
	bool NoError;
	String Message;

	void Set(ui32 ErrorCode)
	{
		NoError = false;
		// TODO: get error string from platform 
	}

	void Unset()
	{
		NoError = true;
		Message = String::Empty;
	}
};

typedef delegate1<void, const TIOStatus&>			ConnectCallback;
typedef delegate1<void, const TIOStatus&>			SendCallback;
typedef delegate2<void, void*,const TIOStatus&>	AcceptedCallback;
typedef delegate2<void, const TIOStatus&, ui32>	ReceiveCallback;




/**
 * Asynchronous Server/Client interface for networking.
 * Like a boss.
 */
class TIOService
{
public:
	enum ServiceStatus
	{
		Waiting,
		Starting,
		Started,
		Stopping,
		Stopped,
	};

	typedef delegate1<void,ServiceStatus> StatusChangedEvent;

	bool						Running;
	ServiceStatus				Status;
	Array<TThread*>			Workers;
	String						Name;
	Array< NSocketAsync* >		Sockets;
	event<StatusChangedEvent>	StatusChanged;

	/**
	 * Adds a listener to server stack. Listeners should be added before server start?
	 * Maybe there is a way to avoid this restriction?
	 * OBOSOLETE?
	 */
	//virtual void AddService( NService* listener ) = 0;

	/**
	 * Creates a thread and starts running on it.
	 * @param workerCount if -1 then it auto determines, 0 no workers
	 */
	virtual void Start( ui32 workerCount = -1) = 0; // TODO: add a virtual function version that takes no parameters

	virtual void Start() = 0;

	/**
	 * Stops all worker threads.
	 */
	virtual void Stop() = 0;

	/**
	 * Stops and starts server back.
	 */
	virtual void Restart() = 0;

	/**
	 * Creates a socket that can be used with IOService.
	 * TODO: make this factory instead?
	 */
	virtual NSocketAsync* CreateSocket(AddressFamilies _AddressFamily, SocketTypes _SocketType, ProtocolTypes _ProtocolType) = 0;

	inline NSocketAsync* CreateSocketTCP()
	{
		return CreateSocket(A_InterNetworkV4, S_Stream, P_TCP);
	}

	inline NSocketAsync* CreateSocketUDP()
	{
		return CreateSocket(A_InterNetworkV4, S_DataGram, P_UDP);
	}

	/**
	 * Converts conventionally created socket to be able to used by this ioService.
	 */
	//virtual NSocket* ConvertSocket(NSocket* oldSocket) = 0;

	virtual void Disconnect( NSocketAsync* Client, bool Graceful = false) = 0;

	virtual void AcceptAsync ( NSocketAsync* sck, NSocketAsync* target, void* object,  NPacket* first, AcceptedCallback* Callback ) = 0;

	virtual void ConnectAsync ( NSocketAsync* sck, NEndPoint& endpoint, ConnectCallback* Callback ) = 0;

	virtual void SendAsync ( NSocketAsync* sck, NPacket* packet , SendCallback* Callback) = 0;

	virtual void RecvAsync ( NSocketAsync* sck, NPacket* packet, ReceiveCallback* Callback ) = 0;

protected:
	inline void ChangeStatus( ServiceStatus newStatus )
	{
		Status = newStatus;
		StatusChanged.call(newStatus);
	}
};

class NSocketAsync: public NSocket
{
public:
	TIOService* Service;

	NSocketAsync(AddressFamilies _AddressFamily, SocketTypes _SocketType, ProtocolTypes _ProtocolType,SOCKET _Socket = INVALID_SOCKET): NSocket(_AddressFamily, _SocketType, _ProtocolType, _Socket)
	{

	}

	inline void BeginAccept ( NSocketAsync* target,void* object, NPacket* first,   AcceptedCallback* Callback)
	{
		Service->AcceptAsync(this, target , object, first, Callback);
	}

	inline void BeginConnect( NEndPoint& endpoint, ConnectCallback* Callback )
	{
		Service->ConnectAsync(this, endpoint, Callback);
	}

	inline void BeginSend(NPacket* packet, SendCallback* Callback)
	{
		Service->SendAsync(this,packet,Callback);
	}

	inline void BeginReceive(NPacket* packet, ReceiveCallback* Callback)
	{
		Service->RecvAsync(this, packet, Callback);
	}
};


/// TODO: SOCKET POOL
/// TODO: BUFFER POOL
/// TODO: BYTE QUEUE

#ifdef WIN32
#include "tioserviceiocp.h"
#else

#endif

#endif