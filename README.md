# TinyEvent
**TinyEvent** is a C++11 single header, lightweight, thread safety and easy to use for event-delegate mechanism.

# Notes

The main class is `stubbornhuang::Event`，`stubbornhuang::Event` is a template class that support variadic paramter template and any return type. 

This class defaults to multi-thread security,  if you wish to disable multi-thread security, you can do so by defining the `EVENT_NO_THREAD_SAFETY` macro before including `event.h`.

# How to Use

Firstly include the `event.h` in your project

```cpp
#include "event.h"
```

Then understand how to use this event class through the following code.

##  Use example

```cpp
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
```

**Example Out**

```
-----Add Delegate-----
Sum = 6
Sub = -4
a = 1 b = 5

-----Remove Delegate-----
Sub = -4
a = 1 b = 5
```



