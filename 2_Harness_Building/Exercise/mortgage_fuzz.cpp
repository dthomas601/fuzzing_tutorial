#include <iostream>
#include "mortgage.h"
#include <sstream>
#include <iomanip>
#include <limits>



using namespace std;

//Function used to determine the number of symbol delimited inputs that have been provided in a string
int counter(string fuzz_string, char c = ' '){

	int count = 0;
	for (int i=0;i<fuzz_string.length();i++){
		if (c == fuzz_string[i])
			count++;
	}
	return count;
}

bool isDouble( string myString ) {
    std::istringstream iss(myString);
    long double ld;
    iss >> noskipws >> ld; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}

bool isNumber( string myString ) {
    std::istringstream iss(myString);
    int i;
    iss >> noskipws >> i; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}

// Function used to check if string is a valid float value.
bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size){

	// Cast the fuzzing data into a string
	string fuzz_strings = string(reinterpret_cast<const char*> (Data),Size);
	string tok;
    stringstream ss(fuzz_strings);

    int year_length ; float rate; long double amount,down;

    // Check to ensure there are four input options provided
	if (counter(fuzz_strings) == 4){

		// Get next string with ',' used as the delimiter
		ss >> tok;

		// Make sure the string is a number and not empty
		if (isNumber(tok) ){

			// Make sure the int value is within the correct limit
			if (stoi(tok) <= 0 )
				return 0;

			year_length = stoi(tok); 
			cout << "Year length "<<year_length<<endl;
			
			ss >> tok;

			if (isFloat(tok) ){

				if (stof(tok) <= 0 )
					return 0;

				rate = stof(tok);
				cout << "Rate "<<rate<<endl;
				
				ss >> tok;

				if (isDouble(tok) ){

					if (stold(tok) <= 0 )
						return 0;

					amount = stold(tok);

					// Set the format of the value that will get printed.
					cout.setf(ios::fixed, ios::floatfield);
		   			cout.setf(ios::showpoint);

					cout << "Loan amount $"<<setprecision(2)<<amount<<endl;

					ss >> tok;

					if (isDouble(tok) ){

						if (stold(tok) < 0 )
							return 0;

						down = stold(tok);
						cout << "Down payment amount $"<<down<<endl;


						// Try statement to test the Mortgage class
						try{
							// Create a Mortgage object and use the variables that were collected previously to test it.
							Mortgage loan1 = Mortgage(year_length,rate,amount,down);
							cout << "Get monthly payment: $" <<loan1.get_monthly_payment() <<endl;
						}
						catch (char const* s){
							cout << s <<endl;
							return 0;
						}

					}
				}

			}

		}

	}
	
	return 0;
}
