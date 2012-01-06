#ifndef NSERVICEHTTPBASIC_H
#define NSERVICEHTTPBASIC_H

#include "nserver.h"

class NServiceHTTP: public NService
{
private:
	TString ServerVersion;

public:

	TString RootFolder;

	NServiceHTTP();

	void Connected		(NSocket* Client);
	void Disconnected	(NSocket* Client);
	void Received		(NSocket* Client, NPacket* Packet);
	void Sent			(NSocket* Client, NPacket* Packet);
};


#endif