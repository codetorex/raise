#ifndef NSOCKET_H
#define NSOCKET_H

#include "tioservice.h"
#include "texception.h"
#include "naddress.h"
#include "npacket.h"
#include "nsocketdefs.h"


#define MAX_BUFF_SIZE		8192

#include "tplatform.h"

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#define INVALID_SOCKET (SOCKET)(~0)
#define closesocket	close
typedef int SOCKET;
#include <unistd.h>
#define SOCKET_ERROR (-1)
#endif



/**
 * Berkeley socket implementation.
 */
class NSocket
{
protected:
	TIOService*			Service;

	void ThrowError( const TString& operation )
	{
#ifdef WIN32
		throw Exception( operation , sfs( Platform.GetErrorDescription(WSAGetLastError())) );
#else
		throw Exception(operation);
#endif
	}

	void Initialize(AddressFamilies _AddressFamily, SocketTypes _SocketType, ProtocolTypes _ProtocolType)
	{
		AddressFamily = _AddressFamily;
		SocketType = _SocketType;
		ProtocolType = _ProtocolType;
		DataObject = 0;
		Connected = false;
	}

public:
	SOCKET				Socket;

	NEndPoint			LocalEndPoint;
	NEndPoint			RemoteEndPoint;

	bool				Connected;
	ui32				State;

	AddressFamilies		AddressFamily;
	SocketTypes			SocketType;
	ProtocolTypes		ProtocolType;

	/// Object related to this socket can be hold in this variable
	void*				DataObject;

	void InitializeSocket()
	{
		Socket = socket(AddressFamily,SocketType,ProtocolType);
		if (Socket == INVALID_SOCKET)
		{
			ThrowError("Socket creation failed %");
		}
	}	

	/**
	 * Creates ordinary socket.
	 */
	NSocket(AddressFamilies _AddressFamily, SocketTypes _SocketType, ProtocolTypes _ProtocolType,SOCKET _Socket = INVALID_SOCKET)
	{
		Initialize(_AddressFamily,_SocketType,_ProtocolType);
		Socket = _Socket;

		if (Socket == INVALID_SOCKET)
		{
			InitializeSocket();
		}
	}

	~NSocket()
	{
		if (Socket != INVALID_SOCKET)
		{
			Disconnect();
		}
	}

	void Connect( NAddress4& address, ui16 port)
	{
		NEndPoint p(address,port);
		Connect(p);
	}

	void Connect( NEndPoint& endPoint )
	{
		if (Connected)
		{
			Disconnect();
		}

		if (Socket == INVALID_SOCKET)
		{
			throw Exception("Socket not initialized");
		}

		sockaddr_in sockin;
		sockin.sin_family = AddressFamily;
		sockin.sin_port = htons(endPoint.Port);
		sockin.sin_addr.s_addr = endPoint.Address.Address;

		int r = connect(Socket,(sockaddr*)&sockin, sizeof(sockaddr_in));
		if (r == SOCKET_ERROR)
		{
			Disconnect();
			ThrowError("Connection failed %");
		}

		RemoteEndPoint = endPoint;

		Connected = true;
	}

	void Connect ( TArray<NAddress4*>& addresses, ui32 port )
	{
		NEndPoint p;
		p.Port = port;

		Exception* excp = 0;

		TArrayEnumerator<NAddress4*> ae(addresses);
		while(ae.MoveNext())
		{
			try
			{
				p.Address = ae.Current->Address;
				Connect(p);
				excp = 0;
				break;
			}
			catch (Exception& e)
			{
				excp = &e; // TODO: copying maybe necessary
			}
		}

		if (excp != 0)
		{
			throw *excp;
		}
	}

	void Disconnect()
	{
		closesocket(Socket);
		Connected = false;
		Socket = INVALID_SOCKET;
	}

	inline int Send( NPacket* data )
	{
		return send(Socket,(char*)data->Data,data->Length,0);
	}

	inline int Receive( byte* pData, ui32 Length)
	{
		return recv(Socket,(char*)pData,Length,0);
	}

	inline int Receive( NPacket* data )
	{
		return Receive(data->Data,data->Capacity);
	}

	inline void Bind( NEndPoint& endpoint )
	{
		sockaddr_in ep;
		ep.sin_family = AddressFamily;
		ep.sin_port = htons(endpoint.Port);
		ep.sin_addr.s_addr = endpoint.Address.Address;

		int r = bind(Socket,(sockaddr*)&ep, sizeof(ep));
		if (r == SOCKET_ERROR)
		{
			Disconnect();
			ThrowError("Bind failed: %");
		}

		LocalEndPoint = endpoint;
	}

	void Listen( ui32 backlog )
	{
		int r = listen(Socket,backlog);
		if (r == SOCKET_ERROR)
		{
			Disconnect();
			ThrowError("Listen failed: %");
		}
	}

	NSocket* Accept()
	{
		sockaddr ep;
		socklen_t epsize;

		SOCKET r = accept(Socket,&ep,&epsize);

		if( r == INVALID_SOCKET )
		{
			Disconnect();
			ThrowError("Accept failed: %");
		}

		NSocket* nsck = new NSocket(AddressFamily,SocketType,ProtocolType,r);

		// TODO: write a ipV6 and ipV4 supporting class that parses these stuff.
		/*switch(ep.sa_family)
		{
		case A_InterNetworkV4:
				sockaddr_in* k = (sockaddr_in*)&ep;
				NAddress4 addr = k->sin_addr.s_addr;
				nsck->RemoteEndPoint.Address = addr;
				nsck->RemoteEndPoint.Port = k->sin_port;
			break;

		case A_InterNetworkV6:
				
			break;
		}*/

		throw NotImplementedException(__FILE__,__LINE__);


	}

	inline void BeginAccept ( NSocket* target,void* object, NPacket* first,   AcceptedCallback* Callback)
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

#endif