#include "http.h"
#include <iostream>
#include "mime_type.h"

void http::httpHandler(Connection * conn){
	Request request;
	
	char a[2048];
	conn->readonly(a,2048);
	std::cout<<a<<std::endl;
	
	//First part
	auto s = conn->readline();
	//example:
	//GET / HTTP1/1
	auto requesthead = split(s,' ');
	if(requesthead.size()!=3){warning("headerror");}
	
	request.method = getMethod(requesthead[0].c_str());
	request.protocol = getProtocol(requesthead[2].c_str());
	
	if(request.protocol!=Protocol::HTTP1_1){
		warning("Not supported protocol");
	}
	
	
	s = conn->readline();
	std::cout<<s<<std::endl;
	s = conn->readline();
	std::cout<<s<<std::endl;
	
	
	char test[] = "HTTP/1.1 404 Not Found\n";
	conn->write(test,sizeof(test));
	conn->close();
	}