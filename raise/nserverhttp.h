#ifndef NSERVERHTTP_H
#define NSERVERHTTP_H

#include "tioservice.h"


class NHTTPServer;
class TFileStream;

class NHTTPSession
{
public:
	NSocket* Socket;
	NHTTPServer* Server;
	TIOService* Service;

	byte ReceiveBuffer[8192];
	NPacket ReceivePacket;

	byte SendBuffer[8192];
	NPacket SendPacket;

	TFileStream* ReadStream;

	NHTTPSession(NHTTPServer* _Server, TIOService* _Service)
	{
		Server = _Server;
		Service = _Service;
		Socket = Service->CreateSocketTCP();
		ReceivePacket.Use(ReceiveBuffer,8192);
		SendPacket.Use(SendBuffer,8192);

		ReadStream = 0;
	}

	~NHTTPSession();

	void BeginReceiving();

	void HandleReceive(const SystemError& err, ui32 dataReaded);

	void HandleSend(const SystemError& err);
};

class NHTTPServer
{
public:
	NSocket* Listener; // TODO: make acceptor class
	TIOService* Service;
	ui16 Port;

	TString ServerName;
	TString ServerVersion;
	TString RootFolder;

	NHTTPServer(TIOService* _Service, ui16 _Port);

	void Start()
	{
		NEndPoint endpoint;
		endpoint.Address = "192.168.2.2";
		endpoint.Port = 80;

		Listener = Service->CreateSocketTCP();
		Listener->Bind( endpoint );
		Listener->Listen(16);

		BeginAccepting();
	}

	void BeginAccepting()
	{
		NHTTPSession* new_client = new NHTTPSession(this,Service);
		Listener->BeginAccept(new_client->Socket,new_client,&new_client->ReceivePacket, GetHandler(this,&NHTTPServer::HandleAccept));
	}

	void HandleAccept(void* object, const SystemError& err)
	{
		NHTTPSession* sess = (NHTTPSession*)object;
		if (err.OSErrorID == 0)
		{
			sess->BeginReceiving();
		}
		else
		{
			delete sess;
		}

		BeginAccepting();
	}
};


#endif