#pragma once
#include <string>
#ifndef __DREDIKI_FILE__
#define __DREDIKI_FILE__
namespace file{
	int fileSize(const char* filename); 
	std::string fileType(std::string filename);
}
#endif