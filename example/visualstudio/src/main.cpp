#include <iostream>

#include "event.h"

void Sum(int a, int b)
{
	std::cout << "Sum = " << a + b << std::endl;
}

class Example
{
public:
	Example()
	{

	}
	virtual~ Example()
	{

	}

	void Print(int a, int b)
	{
		std::cout << "a = " << a <<" " << "b = " << b << std::endl;
	}
};


int main()
{
	stubbornhuang::Event<void(int, int)> event;

	std::cout << "-----Add Delegate-----" << std::endl;
	int sum_fuc_delegate_id = event.AddDelegate(Sum); // static function

	event += [](int a, int b) { std::cout << "Sub = " << a - b << std::endl; }; // lambda function

	Example example;
	event += std::bind(&Example::Print, example, std::placeholders::_1, std::placeholders::_2); // class member function

	event.Invoke(1,5);


	std::cout<<std::endl << "-----Remove Delegate-----" << std::endl;
	event -= (sum_fuc_delegate_id);

	event.Invoke(1, 5);

	return 0;
}