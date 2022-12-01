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


