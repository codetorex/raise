#ifndef NSOCKET_H
#define NSOCKET_H



class NSocket
{
public:
	enum SocketTypes
	{
		Stream = SOCK_STREAM,
		DataGram = SOCK_DGRAM,
		Raw = SOCK_RAW,
		RDM = SOCK_RDM, // reliably-delivered message
		SeqPacket = SOCK_SEQPACKET,
		Unknown = MAXDWORD,
	};

	enum AddressFamilies
	{
		Unspecified = AF_UNSPEC,
		InterNetworkV4 = AF_INET,
		InterNetworkV6 = AF_INET6,
		IPX = AF_IPX,
	};

private:
	static bool Initialized;

	void InitializeSockets();

public:

	NEndPoint LocalEndPoint;

	NEndPoint RemoteEndPoint;

	bool get_SupportIPV4();

	bool get_SupportIPV6();





};


#endif