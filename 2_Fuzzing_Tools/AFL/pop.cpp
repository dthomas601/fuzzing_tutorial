#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


using namespace std;


int main() {


	string data;

	cout << "enter input string: ";
	getline(cin, data);
	
	if (sizeof(data) >= 4){

		// Test to find the input to get to the assert statement
		if(data[0] == 'p') {
				if(data[1] == 'o') {
						if(data[2] =='p') {
								if(data[3] == '!') {
										assert(0);
								}
						}
				}
		}
	}
	
	
	return 0;
	
}