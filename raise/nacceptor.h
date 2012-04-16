#ifndef NACCEPTOR_H
#define NACCEPTOR_H

#include "nsocket.h"
#include "tioservice.h"

/**
 * 
 */
class NAcceptor
{
public:
	NSocket* Listener;
	TIOService* Service;

	NEndPoint ListeningEndpoint;

	bool Accepting;

	NAcceptor()
	{
		Accepting = false;
	}

	NAcceptor( TIOService* _Service, NEndPoint& endpoint)
	{
		Accepting = false;
		Initialize( _Service, endpoint );
	}

	void Initialize( TIOService* _Service, NEndPoint& endpoint )
	{
		if (Accepting)
		{
			throw Exception("Already listening on socket");
		}

		Service = _Service;

		ListeningEndpoint = endpoint;

		Listener = Service->CreateSocketTCP();
		Listener->Bind( ListeningEndpoint );
		Listener->Listen(16);

		Accepting = true;
	}

	void BeginAccept(NSocket* target, void* object, NPacket* first, AcceptedCallback* cb )
	{
		Listener->BeginAccept(target,object,first,cb);
	}

	void Finalize()
	{
		Listener->Disconnect();
		delete Listener;
		Accepting = false;
	}
};

#endif