#ifndef NSOCKETDEFS_H
#define NSOCKETDEFS_H


#ifdef LINUX
#define IPPROTO_IPV4	4
#endif

enum SocketTypes
{
	S_Stream = SOCK_STREAM,
	S_DataGram = SOCK_DGRAM,
	S_Raw = SOCK_RAW,
	S_RDM = SOCK_RDM, // reliably-delivered message
	S_SeqPacket = SOCK_SEQPACKET,
	S_Unknown = MXDWORD,
};

enum AddressFamilies
{
	A_Unspecified = AF_UNSPEC,
	A_InterNetworkV4 = AF_INET,
	A_InterNetworkV6 = AF_INET6,
	A_IPX = AF_IPX,
};

enum ProtocolTypes
{
	P_IP = IPPROTO_IP,
	P_TCP = IPPROTO_TCP,
	P_UDP = IPPROTO_UDP,
	P_IPv4 = IPPROTO_IPV4,
	P_IPv6 = IPPROTO_IPV6,
	P_ICMP = IPPROTO_ICMP,
	P_IGMP = IPPROTO_IGMP,
	P_RAW = IPPROTO_RAW,
	// TODO: extend these
};

#endif