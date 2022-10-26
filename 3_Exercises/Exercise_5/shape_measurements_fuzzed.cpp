#include<iostream>
#include "shape_measurements.h"
#include<assert.h>
#include<sstream>

//measurements

using namespace std;

int countWords(string str)
{
    // Breaking input into word
    // using string stream
   
    // Used for breaking words
    stringstream s(str);
   
    // To store individual words
    string word;
 
    int count = 0;
    while (s >> word)
        count++;
    return count;
}

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

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {

	string in_string;
	int in_val;
	bool done = false;
	
	string fuzz_strings = string(reinterpret_cast<const char*> (Data),Size);

	int count = countWords(fuzz_strings);
	
	if (count >= 2){

		stringstream ss(fuzz_strings);

		//do {
		
		cout << "Hello, lets measure some shapes.."<<endl;
		cout << "Choose a shape." <<endl;
		cout << "1: Circle\n2: Square\n3: Rectangle" <<endl;
		cout << "What shape do you choose? "<<endl;
		ss >> in_string;
		cout << in_string<<endl;
		
				
		if (isNumber(in_string)){
			
			cout << in_string << " is a number"<<endl;
		
			in_val = stoi(in_string);
		
			switch(in_val){
			
				case 1:
					cout << "Count was: "<<count<<endl;
					if (count == 2){
				
						double diameter;
						cout <<"Lets measure a circle."<<endl;
						cout <<"Provide a diameter: "<<endl;
						ss >> diameter;
						
						
						cout<<"Diameter was "<<diameter<<endl;

						if (diameter){

							cout << "The circumference is "<<circumference(diameter)<<endl;;
							cout << "The area is "<< c_area(diameter/2) <<endl;
							done = true;
						}
						
					}
					break;
					
				case 2:
					if (count == 2){

						float sq_length;
						cout <<"Lets measure a square."<<endl;
						cout <<"Provide a length for one side: "<<endl;
						ss >> sq_length;
						cout<<"Sq_length was "<<sq_length<<endl;
						
						if (sq_length){

							cout << "The area for this square is "<< area(sq_length) <<endl;
							done = true;
						}
					}

					break;
				
				
				case 3:
				
					if (count == 3){

						float rec_length,rec_width;

						cout <<"Lets measure a rectangle."<<endl;
						cout <<"Provide a length for one side: "<<endl;
						ss >> rec_length;
						
						cout <<"Provide a width for one side: "<<endl;
						ss >> rec_width;
						cout<<"rec_length was "<<rec_length<<endl;
						cout<<"rec_width was "<<rec_width<<endl;
						if (rec_length && rec_width){

							cout << "The area for this rectange is "<< area(rec_length,rec_width) <<endl;
							done = true;
						}
					}

					break;
				
				
				case 100:
					assert(0);
				
				default :
					cout <<"Not an expected value. Lets try again."<<endl;
			}
		
		}
	}
	return 0;
}