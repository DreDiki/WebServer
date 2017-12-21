#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <mutex>

#ifndef __DREDIKI_CONNECTION__
#define __DREDIKI_CONNECTION__
using std::string;
class Connection{
public:
	int read(char * text, int size);
	int readonly(char * text, int size);
	string readline(char delim='\n');
	bool write(const char * text, int size);
	void close();
//private:
	bool available;
	int connfd;
	char ipaddr[16];
	sockaddr_in clientaddr;
	socklen_t clientaddr_len=sizeof(sockaddr_in);
	
};

#endif