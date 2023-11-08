#include <iostream>
#include <algorithm> //for ranges
#include "functions.h"

int arrayFunctions() {

const int SIZE = 5;
int arr[SIZE] = { 5, 4, 3, 2, 1 };

void initializeArray(); {
	//Initializes the array
	for (int i = 0; i < SIZE; i++)
	{
		arr[i] = 5 - i;
	}
}

void printArray(); {
	// print sorted array
	for (int i = 0; i < SIZE; i++)
	{
		std::cout << arr[i] << " ";
	}
}

void sortingArray(); {
	//sorts the array
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
}
return 0;
}