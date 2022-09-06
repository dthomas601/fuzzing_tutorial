/*
Trivial example to highlight what happens when you have a interger overflow with positive numbers.

*/

#include<iostream>

using namespace std;


int main()
{
	
	int x,y,sum;
	
	cout << "Lets do some addition!!"<<endl;
	cout << "Please enter a positive number " <<endl;
	cin >> x;
	cout << "Please enter another positive number " <<endl;
	cin >> y;
	
	sum = x+y;
	
	if(sum < x || sum < y){
		
		cout << "x was " << x <<endl;
		cout << "y was " << y <<endl;
		cout << "Sum is "<< sum <<endl;
		cout << "That seems weird. Did we have an overflow?" << endl;
	
	}
	
	else{
		
		cout << "The sum is "<<sum<<endl;
	}
	
	return 0;
}


/*

Lets do some addition!!
Please enter a positive number
8767579
Please enter another positive number
586978767564
x was 8767579
y was 2147483647
Sum is -2138716070
That seems weird. Did we have an overflow
*/
