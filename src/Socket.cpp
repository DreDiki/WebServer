#include "Socket.h"
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include "util.h"

void Socket::begin(int port){
	using std::strerror;
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		error("Socket Error");
	}
	int flag=1;
	sockaddr_in servaddr={};
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1) {
        error("Set socket option error: %s (errno: %d)",strerror(errno),errno);
    }
	if(bind(listenfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        error("Bind socket error: %s (errno: %d)",strerror(errno),errno);
    }
    if(listen(listenfd, maxc) == -1) {
        error("Listen socket error: %s (errno: %d)",strerror(errno),errno);
    }
	info("Start listening at port: %d",port);
	listener = std::thread(&Socket::loop,this);
	//close(connfd);
    //close(listenfd);
	/**
			char buff[102400];
		int recvbytes = recv(connfd, buff, sizeof(buff), 0);
        if(recvbytes == -1) {
            printf("recv message error: %s (errno: %d)\n",strerror(errno),errno);
            ;
        } else if(recvbytes == 0) ;
        else printf("recv message: %s\n",buff);
		**/
	
}

void Socket::sethandler(void (*handler)(Connection * conn)){
	this->handler = handler;
}
void Socket::loop(){
	while(!shouldstop){
		Connection c = {};
		//std::cout<<c.clientaddr_len<<std::endl;
		if((c.connfd = accept(listenfd, (struct sockaddr*)&c.clientaddr, &c.clientaddr_len)) == -1) {
			if(shouldstop)return;
			error("Accept socket error: %s (errno: %d)",strerror(errno),errno);
			continue;
		}
		c.available = true;
		inet_ntop(AF_INET, &c.clientaddr.sin_addr, c.ipaddr, 16); 
		//char ip_addr[16];
		//std::cout<<ip_addr<<ntohs(clientaddr.sin_port);
		info("Client %s:%d connected", c.ipaddr, ntohs(c.clientaddr.sin_port));
		connections.push_front(c);
		if(handler){
			//handler(connections.front());
			std::thread work(handler,&connections.front());
			work.detach();
		}else{
			//do nothing
		}
	}
}

void Socket::stop(){
	shouldstop= true;
	shutdown(listenfd,SHUT_RDWR);
	listener.join();
}