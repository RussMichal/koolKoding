// Fibonacci 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

void tempConvert()
{
	std::cout << "temp F please: ";
	float tempF;
	std::cin >> tempF;

	float tempC = tempF - 32 * (5 / 9.0);
	std::cout << "your temp in C is " << tempC << std::endl;
}

void ImaniConvertFtIn()
{
	std::cout << "Hello" << std::endl;
	std::string name;
	float ft_height;
	float in_height;

	std::cout << "Please enter your name: " << std::endl;
	std::cin >> name;
	std::cout << "Please enter your inches in inches: " << std::endl;
	std::cin >> in_height;
	ft_height = in_height / 12.0f;
	std::cout << "Your name is " << name << std::endl;
	std::cout << "Your height is " << in_height << " inches" << std::endl;
}

void ImaniConvertKgLb()
{
	std::cout << "Hello" << std::endl;
	std::string name;
	float kg_weight;
	float lbs_weight;

	std::cout << "Please enter your name: " << std::endl;
	std::cin >> name;
	std::cout << "Please enter your weight in lbs: " << std::endl;
	std::cin >> lbs_weight;
	kg_weight = lbs_weight / 2.2046f;
	std::cout << "Your name is " << name << std::endl;
	std::cout << "You weight " << kg_weight << " kg" << std::endl;
}

int max(const int a, const int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int max(const int a, const int b, const int c)
{
	if (a > b)
	{
		return max(a, c);
	}
	else
	{
		return max(b, c);
	}
}

int min(const int a, const int b)
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int main() 
{
	int aa = 2;
	int bb = 3;
	int cc = 3;

	int maxvalue = max(aa, bb,cc);

	int minvalue = min(aa, bb);

	maxvalue = max(aa+5, bb);

	minvalue = min(aa+5, bb);
    
	int opt=9;
	while (opt!=0)
	{
		std:: cout << "what do you want to convert? 1.ftin 2.kglb 3.temp\n";
		std::cin >> opt;
		switch (opt)
		{
		case 1:
			ImaniConvertFtIn();
			break;
		case 2:
			ImaniConvertKgLb();
			break;
		case 3:
			tempConvert();
			break;
		}
	}
	//functions
		// arguments
		// return types
		// const 
		//overloading

	//array of like 5 numbers init the array with loop to 5,4,3,2,1
	//print the array <- unsorted print
	//sort the array least to greatest <- hard
	//print the array <- sorted print
	//2-3 functions

    return 0;
}