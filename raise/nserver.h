#ifndef NSERVER_H
#define NSERVER_H

#include "raisetypes.h"
#include "tstring.h"

class TIPAddress
{
public:
	union
	{
		struct  
		{
			byte a1;
			byte a2;
			byte a3;
			byte a4;
		};
		byte Parts[4];
		ui32 Address;
	};
	
};

enum NProtocol
{
	NP_TCP,
	NP_UDP,
};

class NClient
{
public:
	

};

/**
 * Server interface for networking.
 */
class NServer
{
public:
	TString Name;

	/**
	 * Setup function for server. IP will be used for determining which device will get packets.
	 */
	virtual void SetupServer(NProtocol protocol, TIPAddress ip, ui16 port ) = 0;

	virtual void StartServer() = 0;
	virtual void StopServer() = 0;

	/**
	 * Stops and starts server back.
	 */
	virtual void RestartServer() = 0;
};

#endif