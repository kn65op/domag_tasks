#include <iostream>
#include "GmockTest/Adder.h"


int main()
{
	Adder adder;
	
	auto a = 2;
	auto b = 3;
	std::cout << a << "+" << b << "=" << adder.add(a,b) << " ?=? " << (a+b) << "\n";
}