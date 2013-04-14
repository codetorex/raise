#ifndef NSERVERHTTP_H
#define NSERVERHTTP_H

#include "tioservice.h"
#include "nacceptor.h"


class NHTTPServer;
class TFileStream;

class NHTTPSession
{
public:
	NSocketAsync* Socket;
	NHTTPServer* Server;
	TIOService* Service;

	byte ReceiveBuffer[8192];
	NPacket ReceivePacket;

	byte SendBuffer[8192];
	NPacket SendPacket;

	TFileStream* ReadStream;

	TMemberDelegate2<NHTTPSession,void, const SystemError&, ui32> MReceiveCallback;
	TMemberDelegate1<NHTTPSession,void, const SystemError&> MSendCallback;

	NHTTPSession(NHTTPServer* _Server, TIOService* _Service): 
				MReceiveCallback(this, &NHTTPSession::HandleReceive),
				MSendCallback(this, &NHTTPSession::HandleSend)
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
	NAcceptor Listener; // TODO: make acceptor class
	TIOService* Service;
	ui16 Port;

	TString ServerName;
	TString ServerVersion;
	TString RootFolder;

	TMemberDelegate2<NHTTPServer,void, void*, const SystemError&> MyCallback;

	NHTTPServer(TIOService* _Service, ui16 _Port);

	void Start( NEndPoint& endpoint )
	{
		Listener.Initialize(Service, endpoint);
		BeginAccepting();
	}

	void BeginAccepting()
	{
		NHTTPSession* new_client = new NHTTPSession(this,Service);
		Listener.BeginAccept(new_client->Socket,new_client,&new_client->ReceivePacket, &MyCallback );
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