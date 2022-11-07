#include <iostream>
#include <stdio.h>
#include<string>
#include <assert.h>

using namespace std;

void printchars(std::string str){
	

	if (str.length() >= 16){
		if (str[15] == 'z'){

                	assert(0); 
        	}

	
		for(int i=0;i<str.length()-1;i++){
			cout<<str[i];
		}

	}
	
}