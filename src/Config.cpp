#include <Config.h>
#include <fstream>
#include "util.h"
//#include <mutex>

Config::Config(){
	
}

Config * Config::obj = nullptr;
std::mutex Config::mtx; 

Config * Config::getInstance(){
	if(!obj){
		mtx.lock();
		if(!obj){
			obj = new Config();
		}
		mtx.unlock();
	}
	return obj;
}
void Config::read(const char * filename){
	configs.clear();
	std::fstream file;
	file.open(filename, std::ios::in);
	std::string s;
	if(file.is_open()){
		while(std::getline(file,s)){
			if(s[0]=='#')continue;//ignore comment
			auto middle = s.find(':');
			if(middle == std::string::npos)continue;
			configs[s.substr(0,middle)]=s.substr(middle+1);
		}
		file.close();
	}
}

std::string Config::getString(const char *s,std::string defaultValue){
	auto got = configs.find (s);
	if(got==configs.end())return defaultValue;
	return got->second;
}

bool Config::getBool(const char *s,bool defaultValue){
	auto got = configs.find (s);
	if(got==configs.end())return defaultValue;
	if(got->second=="true")return true;
	return false;
}

int Config::getInt(const char *s,int defaultValue){
	auto got = configs.find (s);
	if(got==configs.end())return defaultValue;
	try {
		// bitset constructor throws an invalid_argument if initialized
		// with a string containing characters other than 0 and 1
		int i = std::stoi(got->second);
		return i;
	}catch (const std::invalid_argument& ia) {
		error("Invalid setting for %s : %s",s,ia.what());
	}
	return defaultValue;
}