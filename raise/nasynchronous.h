#ifndef NSERVER_H
#define NSERVER_H

#include "nipaddress.h"
#include "npacket.h"
#include "tarray.h"

#define MAX_BUFF_SIZE		8192

class NAsynchronous;
class NService;
class NSocket;


//Platform in depend server interface classes

/**
 * Network listener interface, that listens on server and serves to service.
 */
class NListener
{
public:
	NIPAddress	Device;
	ui16		Port;
	NProtocol	Protocol;
	NService*	Service;

	NListener()
	{

	}

	NListener( const NIPAddress& _ip, ui16 _port, NProtocol _protocol, NService* _service)
	{
		Device = _ip;
		Port = _port;
		Protocol = _protocol;
		Service = _service;
	}
};

/**
 * Network service interface, that can be served on server or can connect other servers.
 */
class NService
{
public:
	TString					Name;

	TArray< NListener* >	Listeners;
	TArray< NSocket* >		Sockets;

	NAsynchronous*			Async;

	/**
	 * When a client connects to server.
	 */
	virtual void Connected		(NSocket* Client) = 0;

	/**
	 * When a client disconnects from server.
	 */
	virtual void Disconnected	(NSocket* Client) = 0;

	/**
	 * Data received from client.
	 */
	virtual void Received		(NSocket* Client, NPacket* Packet) = 0;

	/**
	 * Data sent to client.
	 */
	virtual void Sent			(NSocket* Client, NPacket* Packet) = 0;
};




/**
 * Asynchronous Server/Client interface for networking.
 * Like a boss.
 */
class NAsynchronous
{
public:
	TString Name;

	TArray< NService* > Services;

	/**
	 * Adds a listener to server stack. Listeners should be added before server start?
	 * Maybe there is a way to avoid this restriction?
	 */
	virtual void AddService( NService* listener ) = 0;

	virtual void StartServer() = 0;
	virtual void StopServer() = 0;


	virtual void Disconnect( NSocket* Client, bool Graceful = false) = 0;
	
	/**
	 * You have to take care of the Packet.
	 */
	virtual void Send( NSocket* Client, NPacket* Packet) = 0;


	/**
	 * Stops and starts server back.
	 */
	virtual void RestartServer() = 0;

	virtual void CreateListener( NIPAddress Device, ui16 Port, NProtocol Protocol, NService* Service) = 0;

	virtual bool Connect( NEndPoint EndPoint, NService* Service ) = 0;
};


/// SOCKET POOL
/// BUFFER POOL
/// BYTE QUEUE
class NSocket
{
public:
	enum NSocketStates
	{
		NS_TIMEOUT,

	};

	bool		ConnectionStatus;
	ui32		State;
	NService*	Service;
	NAsynchronous*	Async;

	byte		ReceiveByteBuffer[MAX_BUFF_SIZE];
	byte		SendByteBuffer[MAX_BUFF_SIZE];

	NPacket		ReceiveBuffer;
	NPacket		SendBuffer;

	/// Object related to this socket can be hold in this variable
	void*		DataObject;

	NSocket()
	{
		ReceiveBuffer.Use(ReceiveByteBuffer,MAX_BUFF_SIZE);
		SendBuffer.Use(SendByteBuffer,MAX_BUFF_SIZE);
		DataObject = 0;
	}

	inline void Sent(NPacket* packet)
	{
		Service->Sent(this,packet);
	}

	inline void Received(NPacket* packet)
	{
		Service->Received(this,packet);
	}

	inline void Connected()
	{
		ConnectionStatus = true;
		Service->Connected(this);
	}

	inline void ConnectionFailed()
	{
		ConnectionStatus = false;
		State = NS_TIMEOUT;
		Service->Connected(this);
	}

	inline void Disconnected()
	{
		ConnectionStatus = false;
		Service->Disconnected(this);
	}

	inline void Send(NPacket* Packet)
	{
		Async->Send(this,Packet);
	}

	inline void Disconnect(bool Graceful = false)
	{
		Async->Disconnect(this,Graceful);
	}
};

#ifdef WIN32
#include "nasynchronouswindows.h"
#else
#include "nserverlinux.h"
#endif

#endif