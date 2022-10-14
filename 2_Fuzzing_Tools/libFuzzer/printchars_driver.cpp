#include <stdio.h>
#include<string>

#include "printchar.h"
#include <stdint.h>
#include <stddef.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	
	
	std::string s(reinterpret_cast<const char *>(data), size);
	
	//printf("Size of string: %zu, %s\n",size,s.c_str());		
	printchars(s);

	return 0;
}