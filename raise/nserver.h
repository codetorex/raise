#ifndef NSERVER_H
#define NSERVER_H

#include "nipaddress.h"
#include "npacket.h"
#include "tarray.h"

class NServer;
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
 * Network service interface, that can be served on server.
 */
class NService
{
public:
	TString					Name;

	TArray< NListener* >	Listeners;
	TArray< NSocket* >		Clients;

	NServer*				Server;

	virtual void Connected		(NSocket* Client) = 0;
	virtual void Disconnected	(NSocket* Client) = 0;
	virtual void Received		(NSocket* Client, NPacket* Packet) = 0;
	virtual void Sent			(NSocket* Client, NPacket* Packet) = 0;
};

// TODO: bu nssocketi bi üst seviyeye çýkarmanýn yolunu bulak
class NSocket
{
public:
	bool ConnectionStatus;
	NService* Service;
	
	/// You can point to custom data from this pointer.
	/// OR SHOULD WE MAKE IT DERIVABLE?
	void* DataPtr;

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

	inline void Disconnected()
	{
		ConnectionStatus = false;
		Service->Disconnected(this);
	}

	/*inline void Send(NPacket* Packet)
	{
		Service->Server->Send(this,Packet);
	}

	inline void Recv()
	{
		//Service->Server->
	}*/

	/*virtual void Send() = 0;
	virtual void Received() = 0;*/
};

/**
 * Server interface for networking.
 * Like a boss.
 */
class NServer
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
	 * After a packet passed to this function, you should not modify it.
	 * It maybe even got freed!
	 */
	virtual void Send( NSocket* Client, NPacket* Packet) = 0;


	/**
	 * Stops and starts server back.
	 */
	virtual void RestartServer() = 0;

	virtual void CreateListener( NIPAddress Device, ui16 Port, NProtocol Protocol, NService* Service) = 0;

};

#endif