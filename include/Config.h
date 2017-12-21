#pragma once
#include <unordered_map>
#include <string>
#include <mutex>
#ifndef __DREDIKI_CONFIG__
#define __DREDIKI_CONFIG__
class Config{
public:
	static Config * getInstance();
	void read(const char * s);
	bool getBool(const char *s,bool defaultValue = false);
	int getInt(const char *s,int defaultValue = 0);
	std::string getString(const char *s,std::string defaultValue={});
protected: 
	Config();
private:
	std::unordered_map<std::string,std::string> configs;
	static Config * obj;
	static std::mutex mtx;
};

#endif