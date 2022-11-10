#include <stdio.h>
#include <string>

#include "printchars.h"
#include <stdint.h>
#include <stddef.h>


#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}


int main(int argc, char *argv[]){

//extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	
	//string filename(argv[1]);	
	//std::string s(reinterpret_cast<const char *>(data), size);
	//string file_contents;
	//file_contents = readFileIntoString(filename);


	//printf("Size of string: %zu, %s\n",size,s.c_str());		
	printchars(argv[1]);

	return 0;
}