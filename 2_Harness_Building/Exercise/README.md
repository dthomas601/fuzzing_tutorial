# Harness Creation Exercise

In this exercise libFuzzer will be used to understand the basics of building a fuzzing harness.

In the assignment, different tasks will be outlined for students to complete. The answers provided below, but the goals for the user should be to complete the activities without assistance. Only look at portions of the answers when all other alternatives have been exhausted.

## Fuzz the Mortgage class constructor

The Mortgage class (mortgage.h) has been provided below:

```
#include <iostream>
#include <cmath>
#include <assert.h>

using namespace std;


class Mortgage
{

	public:
		short mortgage_length_years;
		int mortgage_length_months;
		float interest_rate;
		long double home_amount, down_payment;
		long double principle_amount;
		long double monthly_payment();

		long double get_monthly_payment();
		bool loan_duration_valid(int);
		bool valid_principle_amount();

		Mortgage(int year_length, float rate, long double home_value, long double down){

			if (!loan_duration_valid(year_length)){
				cout<< "Loan " << loan_duration_valid(year_length)<<endl;
				throw "Invalid duration: Must be greater than zero";
			}

			else
				mortgage_length_years=year_length;


			interest_rate = rate;
			home_amount=home_value;
			down_payment = down;
			principle_amount = home_value - down_payment;

			mortgage_length_months = mortgage_length_years*12;


			if (!valid_principle_amount()){
				cout<< "Principle Amount " << valid_principle_amount()<<endl;
				throw "Invalid Principle Amount. Must be more than zero.";
			}

		}

};


bool Mortgage::valid_principle_amount(){

	if (principle_amount > 0)
		return true;

	return false;
}

long double Mortgage::get_monthly_payment(){

	if (mortgage_length_years <= 0)
		assert(0);

	return principle_amount * (interest_rate/12) *  pow((1+(interest_rate/12)),mortgage_length_months) / (pow((1+interest_rate/12),mortgage_length_months) - 1);
}


bool Mortgage::loan_duration_valid(int years){

	if (years > 0 )
		return true;

	return false;
}
```

A proof of concept driver program (mortgage_base.cpp) is shown below:

```
#include <iostream>
#include "mortgage.h"

using namespace std;

int main(){


	int year_length = 10; float rate=0.12; long double amount = 1000000;

	Mortgage loan1 = Mortgage(year_length,rate,amount);

	cout << "Get monthly payment: $" <<loan1.get_monthly_payment() <<endl;
	return 0;
}
```


## Task 1: Add the fuzzing harness function

Replace ```int main``` with the function ```extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) ```

<details>
<summary>Solution Task 1</summary>

As an intermediate step the code can look like the following:

```
#include <iostream>
#include "mortgage.h"

using namespace std;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size){
{


	int year_length = 10; float rate=0.12; long double amount = 1000000;

	Mortgage loan1 = Mortgage(year_length,rate,amount);

	cout << "Get monthly payment: $" <<loan1.get_monthly_payment() <<endl;
	return 0;
}
```

</details>

## Task 2: Collecting and Recasting Inputs 

libFuzzer provides inputs via the ```Data``` variable and shows the lengths of these inputs with ```Size```.

Recast these inputs as a ```string``` looking at the example harness from the ```2_Harness_Building``` example.


<details>
<summary>Solution Task 2</summary>

The following line will cast the inputs into a string:

```
string fuzz_strings = string(reinterpret_cast<const char*> (Data),Size);
```

</details>

## Task 3: Seperate strings into object parameters

The ```Mortgage``` constructor takes four values to calculate a monthly mortgage payment: ```year_length, rate, home_value, down```. These correlate to the length of the mortgage, the interest rate that will be used, the value of the home, and finally the down payment that will be provided.

In this example, use a whitespace-delimited string to seperate each parameter. Create a function that checks to make sure the proper number of parameters are being provided by libFuzzer.

<details>
<summary>Solution Task 3</summary>

An example function to check if the string can be turned into arguments is provided below:

```
//Function used to determine the number of symbol delimited inputs that have been provided in a string

int counter(string fuzz_string, char c = ' '){

	int count = 0;
	for (int i=0;i<fuzz_string.length();i++){
		if (c == fuzz_string[i])
			count++;
	}
	return count;
}
```

The ```if``` statement that can be used with this function is as follows:

```
	if (counter(fuzz_strings) == 4){

		// Continue harness building

	}

```

</details>

## Task 4: Ensure the string arguments can be casted to the proper data types

When fuzzing a class there is flexibiliy in if a class should perform its own input validation or if the class is to be tested with valid data types.

For this task the inputs will be validated before passing them to the ```Mortgage``` constructor. For each input the following should occur:

- Split ```fuzz_strings``` from Task 2 input four different variables
	- ```year_length``` as type ```int``` 
	- ```rate``` as type ```float``` 
	- ```amount``` and ```down``` as type ```double```
- Ensure that the numbers are valid
- Ensure that each string is not zero or less. If either condition happens then simply ```return 0``` to stop execution


Hint: For parsing symbol delimited strings, see the following tutorial: 
* https://www.geeksforgeeks.org/stringstream-c-applications/



<details>
<summary>Solution Task 4</summary>

Here are two functions used to ensure that we have valid int, float, and double values.

```
bool isDouble( string myString ) {
    std::istringstream iss(myString);
    long double ld;
    iss >> noskipws >> ld; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}
```
```
bool isNumber( string myString ) {
    std::istringstream iss(myString);
    int i;
    iss >> noskipws >> i; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}

```

```
// Function used to check if string is a valid float value.
bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}
```



The following shows the ```if``` statements used to validate inputs:

```
// Get next string with ',' used as the delimiter
			ss >> tok;

			// Make sure the string is a number and not empty
			if (isNumber(tok) ){

				// Make sure the int value is not zero or less
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

						}
					}
				}
			}
```

</details>


## Task 5: Test Mortgage Constructor

From the value collected previously, pass them as arguments into the ```Mortgage``` constructor. The class does throw errors so they will need to be handled with a ```try/catch``` statement. Review the errors thrown in the ```Mortgage``` class and account for them in the harness.


<details>
<summary>Solution Task 5</summary>

The full harness ```mortgage_fuzz.cpp``` including the try/catch statement is provided below:

```
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
```

</details>

## Task 6: Build and run harness

With the harness created now use it to compile and run the file.


<details>
<summary>Solution Task 6</summary>

The command to compile is shown below:

```
clang++ -g -O1 -fsanitize=fuzzer,address -o mortgage_fuzz.run mortgage_fuzz.cpp
```

To run the fuzzing harness use the following command

```
./mortgage_fuzz.run
```

Given enough time, a integer overflow erorr will be identified in the ```Mortgage``` class being fuzzed.

</details>





