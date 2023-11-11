#include <iostream>
#include <algorithm> //for ranges
#include "functions.h"

const int SIZE = 5;
int arr[SIZE] = { 5, 4, 3, 2, 1 };

void initializeArray(int arr[], int size) {
	for (int i = 0; i < size; i++)
	{
		arr[i] = 5 - i;
	}
}

void printArray(int arr[], int size) {
	// print sorted array
	for (int i = 0; i < SIZE; i++)
	{
		std::cout << arr[i] << " ";
	}
}

void findValue(int arr[], int size) 
{
	std::cout << "enter a number to find\n";
	int number;
	std::cin >> number;
	bool found=false;

	for (int i = 0; i < size; i++)//loop through array
	{
		if (arr[i] == number)//look in array for the number
		{
			found = true;//set bool to true becuase we have found the number
			std::cout << "found " << number << " at index " << i << "\n";//print array index i and the number we found
		}
	}

	if (!found)//!found == not found
	{
		std::cout << "not found\n";
	}
}