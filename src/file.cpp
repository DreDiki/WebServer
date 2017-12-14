#include "file.h"
#include <sys/stat.h> 
int file::fileSize(const char* filename){  
	struct stat statbuf = {};  
	stat(filename,&statbuf);
	return statbuf.st_size;
} 