#include "Connection.h"
#include <iostream>
#include "util.h"
int Connection::read(char * text, int size){
	if(!available)return -1;
	//int recvbytes = recv(connfd, text, size*sizeof(char), 0);
	return recv(connfd, text, size, 0);
}

int Connection::readonly(char * text, int size){
	if(!available)return -1;
	//int recvbytes = recv(connfd, text, size*sizeof(char), 0);
	return recv(connfd, text, size, MSG_PEEK);
}

string Connection::readline(char delim){
	string result;
	if(!available)return result;
	char buffer[1024];
	int size;
	if(recv(connfd, buffer,1, MSG_PEEK)==1&&buffer[0]==delim){
		//error("123");
		recv(connfd, buffer,1,0);
		return string(&delim,1);
	}
	while(true){
		size=recv(connfd, buffer, sizeof(buffer), MSG_PEEK);
		if(size<=0)return result;
		//if(size<1024){
		//	result.append(buffer,size);
		//	break;
		//}
		int i;
		for(i=0;i<size;++i){
			//std::cout<<(int)buffer[i]<<' ';
			if(buffer[i]==delim){
				--i;
				break;
			}
		}
		result.append(buffer,i);
		if(size<1024||i<1024){
			break;
		}
	}
	size=result.size()+2;
	for(;size>1024;size-=1024){
		if(recv(connfd, buffer, sizeof(buffer), 0)!=1024)break;
	}
	//error("%d",size);
	recv(connfd, buffer, size, 0);
	//std::cout<<result;
	return result;
}
bool Connection::write(const char * text, int size){
	if(!available)return false;
	int r = send(connfd, text, size, 0);
	return r==-1;
}

void Connection::close(){
	shutdown(connfd,SHUT_RDWR);
	available = false;
}

