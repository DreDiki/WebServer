#include "server.h"

#include <iostream>
#include <string>
#include "Socket.h"
#include "Connection.h"
#include "http.h"
#include "util.h" 
#include "Config.h"
int main(int argc,char * argv[]){
	Config config;
	config.read("config");
	
	Socket socket;
	socket.sethandler(&http::httpHandler);
	socket.begin(config.getInt("listen",8080));
	
	std::string command;
	bool stop=false;
	while(!stop){
		std::getline(std::cin,command);
		auto requesthead = split(command,' ');
		switch(_hash(requesthead[0].c_str())){
			case "stop"_hash:
				stop = true;
			break;
			default:
				message("Unknown Command");
			break;
		}
		//cout<<command;
	}
	socket.stop();
	return 0;
}