#ifndef TIOSERVICE_H
#define TIOSERVICE_H

#include "naddress.h"
#include "npacket.h"
#include "tarray.h"
#include "tevent.h"
#include "nsocketdefs.h"

class TIOService;
class NService;
class NSocket;
class TThread;
class NSocket;

typedef delegate1<void, const SystemError&>			ConnectCallback;
typedef delegate1<void, const SystemError&>			SendCallback;
typedef delegate2<void, void*,const SystemError&>	AcceptedCallback;
typedef delegate2<void, const SystemError&, ui32>	ReceiveCallback;

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
	TArray<TThread*>			Workers;
	TString						Name;
	TArray< NSocket* >			Sockets;
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
	virtual NSocket* CreateSocket(AddressFamilies _AddressFamily, SocketTypes _SocketType, ProtocolTypes _ProtocolType) = 0;

	inline NSocket* CreateSocketTCP()
	{
		return CreateSocket(A_InterNetworkV4, S_Stream, P_TCP);
	}

	inline NSocket* CreateSocketUDP()
	{
		return CreateSocket(A_InterNetworkV4, S_DataGram, P_UDP);
	}

	/**
	 * Converts conventionally created socket to be able to used by this ioService.
	 */
	//virtual NSocket* ConvertSocket(NSocket* oldSocket) = 0;

	virtual void Disconnect( NSocket* Client, bool Graceful = false) = 0;

	virtual void AcceptAsync ( NSocket* sck, NSocket* target, void* object,  NPacket* first, AcceptedCallback* Callback ) = 0;

	virtual void ConnectAsync ( NSocket* sck, NEndPoint& endpoint, ConnectCallback* Callback ) = 0;

	virtual void SendAsync ( NSocket* sck, NPacket* packet , SendCallback* Callback) = 0;

	virtual void RecvAsync ( NSocket* sck, NPacket* packet, ReceiveCallback* Callback ) = 0;

protected:
	inline void ChangeStatus( ServiceStatus newStatus )
	{
		Status = newStatus;
		StatusChanged.call(newStatus);
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