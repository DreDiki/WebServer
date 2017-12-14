#include <unordered_map>
#include <string>
#pragma once
#ifndef __DREDIKI_CONFIG__
#define __DREDIKI_CONFIG__
class Config{
public:
	void read(const char * s);
	
	bool getBool(const char *s,bool defaultValue = false);
	int getInt(const char *s,int defaultValue = 0);
	std::string getString(const char *s,std::string defaultValue={});
private:
	std::unordered_map<std::string,std::string> configs;
};

#endif