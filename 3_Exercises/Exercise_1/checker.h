#include <iostream>
using namespace std;


/*
Helper library with two functions. 
*/

int count_chars(char* check_string, char check_char){
	
	/*
	count_chars is used to determine the number of times a character appears in 
	a string.
	*/
	
	int counter=0;
	
	for (int i=0;i<strlen(check_string);i++){
		
		if (check_char == check_string[i])
			counter++;
	}
	
	return counter;
}

bool validate_data(string user_data) {
	
	/*
	validate_data is used to ensure that the string provide is of the proper length.
	More that 0, but less than 100.
	*/

	unsigned char length = user_data.length();
	
	cout << "Length "<<int(length)<<endl;

	if(length >= 1 && length <= 99)
		return true;

	else
		return false;

}