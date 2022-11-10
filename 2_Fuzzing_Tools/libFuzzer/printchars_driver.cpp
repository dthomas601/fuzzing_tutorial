#include <stdio.h>
#include<string>

#include "printchars.h"
#include <stdint.h>
#include <stddef.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	
	
	std::string s(reinterpret_cast<const char *>(data), size);
	
	printchars(s);

	return 0;
}