// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//braden
//Make a program that converts feet to inches
void homework()
{
	std::cout << "what to convert 1 for ft to in, 0 for oz to g:\n";
	bool convertType;
	std::cin >> convertType;

	if (convertType)
	{
		std::cout << "Convert feet to inches:\n";
		float Feet;
		std::cin >> Feet;

		float Inches = (Feet * 12.0f);
		std::cout << Feet << " feet is " << Inches << " inches\n";
	}
	else
	{
		std::cout << "enter a measurement in ounces:";
		float oz;
		std::cin >> oz;

		float g = (oz * 28.3495);
		std::cout << "the measurement in grams is: " << g;
	}
}

int main()
{
	//binary
	//more data type
	//8 bits = 1 byte
	char; //1 byte 0-255
	int;  //4 byte
	float; //4 byte

	bool b;//1byte; true not 0 or false=0
	
	short sh;//2 byte
	int i;//4 byte
	long l;//8 byte
	long long ll;//16 byte
	unsigned int ui;//4 byte
	
	float pi = 3.14;  //4 byte
	double dpi = 3.14159; //8 byte

	char; //1 byte 0-255

	char r = 'r';
	char u = 'u';
	char s = 's';

	std::string rus = "rus";
	std::string name = "joan";
	std::string message = "hello, how are you?";

	//if
//T&&T = T
//T&&F = F
//F&&T = F
//F&&F = F

//T||T = T
//T||F = T
//F||T = T
//F||F = F

	int instrumentCount;
	std::cout << "how many instruments do you play?";
	std::cin >> instrumentCount;
	
	bool clarinet;
	std::cout << "do you play clarinet?";
	std::cin >> clarinet;

	bool piano;
	std::cout << "do you play paino?";
	std::cin >> piano;

	if (instrumentCount > 2 && clarinet)
	{
		std::cout << "you play clarinet and ...";
	}
	else if (instrumentCount <= 0)
	{
		std::cout << "you dont play";
	}
	else if ((piano || clarinet) && instrumentCount==1)
	{
		std::cout << "you only play clarinet or paino";
	}

	//switch
	int age = 1;
	//switch
	switch (age)
	{
	case 1:
		std::cout << "age 1\n";
	case 2:
		std::cout << "age 2\n";
		break;
	case 3:
		std::cout << "age 3\n";
		break;
	case 4:
		std::cout << "age 4\n";
		break;
	default:
		std::cout << "age default\n";
		break;
	}


	//tax app example
	int income;
	std::cout << "whats your income?";
	std::cin >> income;

	float taxRate;
	switch (income)
	{
	case 10000:
		taxRate = 0.0;
		break;
	case 20000:
		taxRate = 0.1;
		break;
	case 30000:
		taxRate = 0.2;
		break;
	case 40000:
		taxRate = 1.0;
		break;
	default:
		taxRate = -0.5;
		break;
	}

	std::cout << "your tax rate from switch is: "<< taxRate<<"\n";

	if (income < 10000)
	{
		taxRate = 0.0;
	}
	else if (income > 10000 && income < 20000)
	{
		taxRate = 0.1;
	}
	else if (income > 20000 && income < 30000)
	{
		taxRate = 0.2;
	}
	else if (income >= 40000)
	{
		taxRate = 1.0;
	}
	std::cout << "your tax rate from if is: " << taxRate<<"\n";

	//home work calculator
	//take a number
	//take a operation (-,+,*,/)
	//take another number
	//do the operation on the numbers

	//one number must be less than 10
	//other number must be greater then 10
	//if thats not true then exit
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
