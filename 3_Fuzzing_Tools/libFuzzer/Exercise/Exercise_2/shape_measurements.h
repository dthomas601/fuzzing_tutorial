#include<iostream>
#define _USE_MATH_DEFINES
#include<cmath>
#include<assert.h>

using namespace std;

int circumference(double diameter){
	
	int c = diameter*M_PI;
	if (c<0){
		cout << "Error circumference is "<< c << endl;
		assert(0);
	}
	return c;
}

int c_area(double radius){
	
	int a = M_PI*pow(radius,2);
	
	if (a<0){
		cout << "Error area is "<< a << endl;
		assert(0);
	}
	return a;
}

int area(double length,double width=0){
	
	if (!width)
		return pow(length,2) ;
	else
		return length*width;
	
}

