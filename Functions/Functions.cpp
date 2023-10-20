// Functions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm> //for ranges

// Function to initialize the array with values 5, 4, 3, 2, 1
void initializeArray(int arr[], int size) 
{
    for (int i = 0; i < size; i++) 
    {
        arr[i] = 5 - i;
    }
}

// Function to print the array
void printArray(int arr[], int size) 
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

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

    // Print the sorted array
    std::cout << " ";
    printArray(myArray, size);

    return 0;
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
