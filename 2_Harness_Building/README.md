# Building a fuzz harness

This section provides guidance on how a fuzzing harness should be created.

This course will be taught through examples.

Each of the tools for this course can use the harness function ```LLVMFuzzerTestOneInput```.

An example harness from the libFuzzer documentation shows the following pseudo-code:

```
// fuzz_target.cc
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  DoSomethingInterestingWithMyAPI(Data, Size);
  return 0;  // Values other than 0 and -1 are reserved for future use.
}
```

Another simple example can be found below:

```
#include <stdint.h>
#include <stddef.h>


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size > 0 && data[0] == 'H')
    if (size > 1 && data[1] == 'I')
       if (size > 2 && data[2] == '!')
       __builtin_trap();
  return 0;
}
```

This program basically checks inputs from a fuzzer and crashes the program when the string ```HI!``` is provided.

## Fuzzing Harness Basics


The goal of a harness is to write a small program that can quickly integrate projects that include callable sets of functions. The fuzzer input is provided as parameters to a C function (instead of primarily using STDIN). The program also tries to run as fast as possible without respawning for every input. In the example above the LLVMFuzzerTestOneInput() function takes arguments Data and Size. Data is a fixed-length input that is then passed on to the code to be tested. Size is used to know how large the fixed-length buffer is.

Best practices for a fuzzing environment based on the above-mentioned harness:

* Fuzz libraries and APIs, rather than standalone programs.
* Ensure the program behavior is as deterministic as possible. The same input must result in the same output.
* The called library should avoid exiting (by exit() or raising signals) for valid code paths.
* It should avoid mutating the global state of the program as it will confuse the fuzzer.
* It should evaluate as quickly as possible, returning to the fuzzer.
* It may use threads, but all newly spawned threads should be joined before returning to libFuzzer.

AFL and honggfuzz can better test stand-alone programs compared to libFuzzer. Both tools have options to take input strings from STDIN versus being passed in as parameters. Both tools can also support the above-mentioned format as well. This allows for a single harness to be used for multiple tools.

# Example Program

```
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <regex>

using namespace std; 


class patient 
{ 
	private: 
		char name[30]; 
		unsigned int PID; 
		unsigned int age;
		unsigned int year;
		char email[40];
		bool isValidEmail(const string& email);
		
	public:
		void setName(const char * _name);
		void setID(void);
		void setBirthYear(unsigned int year);
		void setEmail(const char * mail);
		char * getName();
		char * getEmail();
		unsigned int getYear();
		unsigned int getAge();

};

void patient::setBirthYear(unsigned int _year){
	
	year = _year;
}

void patient::setEmail(const char * mail){
	
	if (isValidEmail(mail)){
		
		strcpy(email,mail);
	}
	else{
		cout <<"Not Valid"<<endl;
	}

}

// Function to check the email id
// is valid or not
bool patient::isValidEmail(const string& email)
{
  
    // Regular expression definition
    const regex pattern(
        "(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
  
    // Match the string pattern
    // with regular expression
    return regex_match(email, pattern);
}

char * patient::getName(){
	return name;
}

char * patient::getEmail(){
	return email;
	
}

unsigned int patient::getYear(){
	return year;
}


void patient::setName(const char * _name){
	
	strcpy(name,_name);
	
}
```

This header file contains a class and associated member functions that can be tested. A fuzzing harness will be created to test each of the provided functions.

Our fuzzing harness for this example is provided below:

```
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
```

We will now build and run the file shown above. We will use the standard libFuzzer first.

```
clang++ -g -O1 -fsanitize=fuzzer -o fuzz_class.run fuzz_class.cpp 
```

* -g - Provides source-level debugging information
* -O1 - Sets compiler optimizations for the produced executable
* -fsanitize=fuzzer - Specify that this executable will be built as a fuzzing target
* -o fuzz_class.run - Define the name of the fuzz target.


To run the generated executable, simply perform the following:

```
./fuzz_class.run
```

Additional options called Sanitizers can also be provided to identify other potential vulnerabilities. Below we add the option for using an AddressSanitizer (ASan). It is used to identify potential buffer overflows. It pads stack and heap allocations with "poison memory" and tests if the fuzzer input tries to use these bad areas.

```
clang++ -g -O1 -fsanitize=fuzzer,address -o fuzz_class_asan.run fuzz_class.cpp
```

Again to start the example simply run the generated executable:

```
./fuzz_class_asan.run
```

The crash that is generated should provide an input that describes where buffer overflows could occur.


Problems with the code can be found in the functions ```setName``` and ```setEmail```. Each of these have buffers of fixed size and use ```strcpy``` which has no bounds checking capability.
