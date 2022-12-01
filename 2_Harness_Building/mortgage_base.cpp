#include <iostream>
#include "mortgage.h"

using namespace std;

int main(){


	unsigned short year_length = 10; float rate=0.12; long double amount = 1000000;

	Mortgage loan1 = Mortgage(year_length,rate,amount);

	cout << "Get monthly payment: $" <<loan1.get_monthly_payment() <<endl;
	return 0;
}
