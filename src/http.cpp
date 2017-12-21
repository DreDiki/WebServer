#include "Config.h"
#include "http.h"
#include <iostream>
#include "mime_type.h"
#include "file.h"
#include <fstream>
#include <sstream>

void http::httpHandler(Connection * conn){
	static std::string * root;
	static std::string * f0f;
	if(!root){
		Config * config = Config::getInstance();
		root = new std::string(config->getString("root","./root/"));
		f0f = new std::string(config->getString("404","false"));
	}
	
	Request request;
	char a[2048];
	conn->readonly(a,2048);
	std::cout<<a<<std::endl;
	
	//First part
	auto s = conn->readline();
	//example:
	//GET / HTTP1/1
	auto requesthead = split(s,' ');
	if(requesthead.size()!=3){
		warning("headerror");
		conn->close();
		return;
	}
	
	request.method = getMethod(requesthead[0].c_str());
	request.protocol = getProtocol(requesthead[2].c_str());
	
	if(request.protocol!=Protocol::HTTP1_1){
		warning("Not supported protocol");
		char test[] = "HTTP/1.1 505 HTTP Version not supported\n";
		conn->write(test,sizeof(test));
		conn->close();
		return;
	}
	//read properties
	while(s!="\0"&&s!="\n"){
		//if(s.size()==1)
		//info("%d",s[0]);
		s = conn->readline();
	}
	
	//std::cout<<s<<std::endl;
	if(requesthead[1].back()=='/')requesthead[1].append("index.html");
	std::string filename = *root+requesthead[1];
	std::fstream file;
	file.open(filename, std::ios::in|std::ios::binary);
	if(file.is_open()){
		int filesize = file::fileSize(filename.c_str());
		std::stringstream headstream;
		headstream<<"HTTP/1.1 200 OK\n";//Content-Type:charset=UTF-8\n
		headstream<<"Server:DreDiki HTTP Server\n";
		headstream<<"Content-Length:"<<filesize<<"\n";
		headstream<<"\n";
		std::string head = headstream.str();
		conn->write(&head[0],head.size());
		
		char buffer[1024];
		while(true){
			file.read(buffer,1024);
			if(file){
				conn->write(buffer,sizeof(buffer));
			} else{
				int read = file.gcount();
				if(read>0)
					conn->write(buffer,read);
				break;
			}
		}
		
		file.close();
	}else{
		std::stringstream headstream;
		headstream<<"HTTP/1.1 404 Not Found\n";
		headstream<<"Server:DreDiki HTTP Server\n";
		headstream<<"\n";
		std::string head = headstream.str();
		conn->write(&head[0],head.size());
		if((*f0f)!="false"){
			std::fstream file;
			file.open(*f0f, std::ios::in|std::ios::binary);
			if(file.is_open()){
				char buffer[1024];
				while(true){
					file.read(buffer,1024);
					if(file){
						conn->write(buffer,file.gcount());
					} else{
						int read = file.gcount();
						if(read>0)
							conn->write(buffer,read);
						break;
					}
				}
			}
		}
		
	}
	conn->close();
}