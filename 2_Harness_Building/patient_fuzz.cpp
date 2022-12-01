#include "patient_class.h"
#include <iostream>
#include <sstream>

using namespace std;

// Function to check the string character by character  for
// any non-digits.
bool isNumber(string& str)
{
    for (char const &c : str) {

        // using the std::isdigit() function
        if (std::isdigit(c) == 0)
          return false;
    }
    return true;
}

int counter(string fuzz_string){

	char c=',';
	int count = 0;
	for (int i=0;i<fuzz_string.length();i++){
		if (c == fuzz_string[i])
			count++;
	}
	return count;
}

// Function used for fuzzing
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size){


	// Input provided from from the fuzzer are converted to strings
	string fuzz_strings = string(reinterpret_cast<const char*> (Data),Size);
	
	// Take the strings and pass them to a stringstream to be used for testing
	string tok;
    stringstream ss(fuzz_strings);

    // The string input is delimited by commas. 
    // Test to ensure there are at least 3 commas in the input data.
	if (counter(fuzz_strings) >= 3){
	
		// Create an instance of the class to be tested
		patient p = patient();
		
		// Grab an input from the stringstream and save to tok
		getline(ss, tok,',');
		
		// Place additional checks on the input provided
		if (tok.length() > 0 && !tok.empty()){
			cout <<"Tok was "<<tok<<endl;
			p.setName(tok.c_str());
		}
		else{
			// If the input is not valid return 0. We do not want to exit as the fuzzer would consider this a crash.
			return 0;
		}
		
		getline(ss, tok,',');
		if (tok.length() > 0 &&!tok.empty()){
			p.setEmail(tok.c_str());
		}
		else
			return 0;
		
		getline(ss, tok,',');
		if (tok.length() > 0 && isNumber(tok) && !tok.empty()){
			p.setBirthYear(stoul(tok));
		}
		else
			return 0;

		
		cout << p.getName()<<endl;
		cout << p.getEmail()<<endl;
		cout << p.getYear()<<endl;

	}
	return 0;
}