#ifndef NCLIENT_H
#define NCLIENT_H

#include "nipaddress.h"
#include "tevent.h"

class NClient
{
public:

	SOCKET m_Socket;

	NEndPoint EndPoint;
	bool Connected;

	NClient()
	{
		Connected = false;
	}



	typedef delegate0<void> OnConnectCallback;


	/**
	 * Creates a connect thread and connects to socket.
	 */
	void BeginConnect(NEndPoint _endpoint, OnConnectCallback* cback );

	void Connect(NEndPoint _endpoint)
	{
		if (Connected)
		{
			// close connection
		}

		EndPoint = _endpoint;

		addrinfo hints, *res;

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		TString portstr = TConvert::ToString(EndPoint.Port);
		TString addrstr = EndPoint.Address.ToString();

		getaddrinfo((char*)addrstr.Data, (char*)portstr.Data, &hints, &res);

		m_Socket = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

		if ( connect(m_Socket,res->ai_addr,res->ai_addrlen) == SOCKET_ERROR )
		{
			throw Exception("Connection failed");
		}

		Connected = true;
	}


};

#endif