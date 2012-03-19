#ifndef TIOSERVICE_H
#define TIOSERVICE_H

class TIOService
{
public:
#ifndef WIN32
	HANDLE IOCPHandle;
#else

#endif
	

	/**
	 * Creates a thread and starts running on it.
	 */
	void Start();

	/**
	 * Stops all worker threads.
	 */
	void Stop(bool graceful = false);

	/**
	 * Restarts workers.
	 */
	void Restart();

	/**
	 * Main function of worker threads.
	 */
	void WorkerMain();
};

#endif