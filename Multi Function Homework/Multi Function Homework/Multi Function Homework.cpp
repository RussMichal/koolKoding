// Multi Function Homework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm> //for ranges
#include "functions.h"

int main() {
    const int size = 5;
    int myArray[size];

    // Initialize the array
    initializeArray(myArray, size);

    // Print the unsorted array
    std::cout << " ";
    printArray(myArray, size);

    // Sort the array in ascending order
    std::sort(myArray, myArray + size);
    sortingArray(myArray, size);
    // Print the sorted array
    std::cout << " ";
    printArray(myArray, size);
    return 0;
}