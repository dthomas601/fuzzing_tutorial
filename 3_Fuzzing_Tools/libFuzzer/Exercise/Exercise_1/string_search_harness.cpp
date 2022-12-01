/*
This program is used to count the number of times a characters 
appears in a given string.

Example:

String: "welcome to the real world"
Search character: w

Count: 2
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

#include "checker.h"

using namespace std;


//Function to check the number of inputs provided
int counter(string str){

	string tok;
	stringstream ss(str);
	int the_count = 0;
  
	while(getline(ss, tok,',')) {
	
		
		if (tok.length() > 1){
		
			the_count++;
		}
		
		// If input is not valid then just return 0 so execution will stop
		else {
			return 0;
		}
	
	}
	
	return the_count;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {

//int main(int argc, char* argv[]) {
	string fuzz_strings = string(reinterpret_cast<const char*> (Data),Size);

	int count = counter(fuzz_strings);
	
	if (count == 2){
		
		stringstream ss(fuzz_strings);


		string checked_string;
		char search_char;
		
		// Get user input from STDIN
		
		cout << "Enter the string that will be searched: "<<endl;
		getline(ss, checked_string,',');
		
		
		cout <<"Enter the character that is being searched for: "<<endl;
		ss >> search_char;
		


		//Ensure the input is at least of length 1

		if(checked_string.length() < 1 )  {

			cout <<"The checked string should be at least of length 1. \n";
			cout <<checked_string<< " " <<checked_string.length()<<endl;
			fflush(stdout);
			exit(0);
			
			
		}
		
		//Ensure the char is of length 1.
		
		else if (sizeof(search_char) != 1){
			
			cout <<"The search character should be of size 1"<<endl;
			return 0;	
		}
		
		else{
		
			//Validate the size of the string
			if (!validate_data(checked_string)){
				cout << "Not good input, try again."<<endl;
				cout << "String should be more than 0, but less than 100"<<endl;
			}
			
		
			else{
				
				char buffer[100];
				strcpy(buffer,checked_string.c_str());
				
				cout << "Search char is "<<search_char<<endl;
				cout << "Checked string "<<buffer<<endl;

							
				int counted_chars = count_chars(buffer, search_char);
				cout << "Chars count was: "<< counted_chars<<endl;

			}
		
		}
	}
	
	return 0;

}

