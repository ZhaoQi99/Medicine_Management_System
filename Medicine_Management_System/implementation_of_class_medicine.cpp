// implementation of class Medicine

#include<string>
#include"medicine.h"

using std::string;


Medicine::Medicine(int t_number, string t_name, int t_amount, double t_price)
{
	number = t_number;
	name = t_name;
	amount = t_amount;
	price = t_price;
	accessibility = true;
}

Medicine::Medicine(Medicine & t_medicine)
{
	number = t_medicine.number;
	name = t_medicine.name;
	amount = t_medicine.amount;
	price = t_medicine.price;
	accessibility = t_medicine.accessibility;
}

double Medicine::getPrice()
{
	return price;
}

string Medicine::getName()
{
	return name;
}
