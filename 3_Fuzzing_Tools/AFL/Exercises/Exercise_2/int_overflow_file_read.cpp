/*
Trivial example to highlight what happens when you have a interger overflow with positive numbers.

*/

#include<iostream>
#include<fstream>
#include<unistd.h>
#include <stdlib.h>
#include<string>

extern "C" {
   void adainit (void);
   void adafinal (void);
   int add_func (int,int);
}

using namespace std;


int count = 0;
void number()
{
  count--;
    cout<<"number of lines : " << count << endl;
};

//Function to check the number of lines in a file
int counter(string filepath){
	
	string line;
	ifstream file(filepath);
	if(file.is_open())
	{
        while(!file.eof())
		{
            getline(file,line);
            cout<< line << endl;
            count++;
        }
        file.close();
    }
    number();
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
    

int main(int argc, char** argv)
{
	
	adainit();
	
	int x,y,sum;
	
	
	string line;
    ifstream myfile;
	
	counter(argv[1]);
	
	if (count == 2){
		int answer_array[count];
		
		myfile.open(argv[1]);

		if(!myfile.is_open()) {
		  perror("Error open");
		  exit(EXIT_FAILURE);
		}
	   
		int loop_count=0;
		
		while(getline(myfile, line)) {
			
			if (!isNumber(line) || line.empty()){
				cout << "Invalid input. Exiting."<<endl;
				exit(0);
			}
				
			answer_array[loop_count] = stoi(line);
			loop_count+=1;
		}
		
		if (answer_array[0] > 0 && answer_array[1] > 0){
		
			sum = add_func(answer_array[0],answer_array[1]);
			
			if(sum < answer_array[0] || sum < answer_array[1]){
				
				cout << "Val1 was " << answer_array[0] <<endl;
				cout << "Val2 was " << answer_array[1] <<endl;
				cout << "Sum is "<< sum <<endl;
				cout << "That seems weird. Did we have an overflow?" << endl;
			}
			
			else{
				cout << "The sum is "<<sum<<endl;
			}
		}
	}
	adafinal();
	return 0;
}
