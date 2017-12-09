#include "Connection.h"
#include <list>
#include <thread>
#include <mutex>
#ifndef __DREDIKI_SOCKET__
#define __DREDIKI_SOCKET__
class Socket{

public:
	void begin(int port);
	void sethandler(void (*handler)(Connection * conn));
	void stop();
protected:
	void loop();
private:
	std::thread listener;
	bool shouldstop = false;
	void (*handler)(Connection * conn);
	int listenfd = -1;
	int maxc = 10;
	float lastconntime = 0;
	std::list<Connection> connections;
};

#endif