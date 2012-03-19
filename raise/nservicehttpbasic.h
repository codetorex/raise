#ifndef NSERVICEHTTPBASIC_H
#define NSERVICEHTTPBASIC_H

#include "nasynchronous.h"

class NServiceHTTP: public NService
{
private:
	TString ServerVersion;

public:

	TString RootFolder;

	NServiceHTTP();

	void Connected		(NSocketOLD* Client);
	void Disconnected	(NSocketOLD* Client);
	void Received		(NSocketOLD* Client, NPacket* Packet);
	void Sent			(NSocketOLD* Client, NPacket* Packet);
};


#endif