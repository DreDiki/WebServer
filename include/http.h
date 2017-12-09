#pragma once
#include "Connection.h"
#include <iostream>
#include "util.h"
#ifndef __DREDIKI_HTTP__
#define __DREDIKI_HTTP__

namespace http{
	enum Protocol {HTTP1_0,HTTP1_1,HTTP2};
	enum Method {GET,POST,HEAD,PUT,PATCH,OPTIONS,DELETE};
	struct Request{
		Method method;
		Protocol protocol; 
	};
	
	inline Protocol getProtocol(const char * s){
		switch(_hash(s)){
			case "HTTP/1.0"_hash:
				return Protocol::HTTP1_0;
			case "HTTP/1.1"_hash:
				return Protocol::HTTP1_1;
			case "HTTP/2"_hash:
				return Protocol::HTTP2;
		}
		return {};
	}
	inline Method getMethod(const char * s){
		switch(_hash(s)){
			case "GET"_hash:
				return Method::GET;
			case "POST"_hash:
				return Method::POST;
			case "HEAD"_hash:
				return Method::HEAD;
			case "PUT"_hash:
				return Method::PUT;
			case "PATCH"_hash:
				return Method::PATCH;
			case "OPTIONS"_hash:
				return Method::OPTIONS;
			case "DELETE"_hash:
				return Method::DELETE;
		}
		return {};
	}
	void httpHandler(Connection * conn);

}

#endif