// Fibonacci 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main() 
{
	const int SIZE = 5;
	int arr[SIZE] = { 5, 4, 3, 2, 1 };

	for (int i = 0; i < SIZE - 1; i++)
	{
		for (int j = 0; j < SIZE - i - 1; j++) {
			if (arr[j] > arr[j + 1])
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}

	// print sorted array
	for (int i = 0; i < SIZE; i++)
	{
		std::cout << arr[i] << " ";
	}

    return 0;
}