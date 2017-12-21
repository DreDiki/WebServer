#include "file.h"
#include <sys/stat.h> 
#include "util.h"
int file::fileSize(const char* filename){  
	struct stat statbuf = {};  
	stat(filename,&statbuf);
	return statbuf.st_size;
}

std::string file::fileType(std::string filename){
	std::size_t start;
	if((start=filename.find_last_of('.',0)+1)!= 0 && start!=std::string::npos){
		//error(filename.substr(start).c_str());
		return filename.substr(start);
	}
	return {};
}