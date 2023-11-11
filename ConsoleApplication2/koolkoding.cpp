// Fibonacci 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
void max(int a, int b, int c)//max int int int -> maxintintint
{}
void max(int a, float b)//max int float -> maxintfloat
{}
int max(int& a, int b=6)//max int int -> maxintint
{
	a = 9;
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
float max(const float& a, const float b)//max float float -> maxfloatfloat
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

int main()
{
	int num1 = 3;
	int num2 = 5;
	int maxnum = max(num1, num2);//max int int -> maxintint
	
	float num1f = 3;
	float num2f = 5;
	float maxnumf = max(num1f, num2f);//max float float -> maxfloatfloat

	// defult arguments 
	//overload
	
	//const
	//pass by value pass by reference
		//pointers

	//make 2 arrays
	//init them to random number with rand() or by hard coding them with int x[10] ={0,1,2,3,4,5,6,7,8,9}
	//find the average of array1 and array2
	//print array 1 avg > array 2 or array 2 avg > array 1
	//default arg for array size
}