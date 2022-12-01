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