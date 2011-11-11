#ifndef NSERVERWINDOWS_H
#define NSERVERWINDOWS_H

#include "nserver.h"
#include "tarray.h"
#include "tthread.h"
#include "tcriticalsection.h"

// NCLIENT FACTORY?

/**
 * IOCP based scalable windows server.
 * Everything needs to be scalable nowadays, who needs old techniques anyway?
 */
/*class NServerWindows: public NServer
{
private:
	WSAEVENT CleanupEvent[1];
	void Cleanup();

	HANDLE IOCPHandle;
	TCriticalSection ContextListCS;		// guard access to the global context list

	bool Restart;
	bool EndServer;
	int	BestWorkerCount;

	SOCKET ListenSocket;

	bool CreateListenSocket();

	bool CreateAcceptSocket(bool updateIOCP);

	SOCKET CreateSocket();

public:
	NProtocol			ServerProtocol;
	ui16				ServerPort;
	TIPAddress			OutputDeviceIP;

	TArray< TThread* >	WorkerThreads;

	TThread*			MainThread;

	void SetupServer(NProtocol protocol, const TIPAddress& ip, ui16 port );

	void StartServer();

	void MainThreadFunction();
	void WorkerThreadFunction();
};*/

#endif