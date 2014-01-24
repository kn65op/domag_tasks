#include <iostream>

int add(int, int);

int main()
{
	
	auto a = 2;
	auto b = 3;
	std::cout << a << "+" << b << "=" << add(a,b) << " ?=? " << (a+b) << "\n";
}