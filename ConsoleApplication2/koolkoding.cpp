// Functions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm> //for ranges

#include "Functions.h"
//   void initializeArray(int arr[], int size);
//   void printArray(int arr[], int size);

//preproc
//resolve any # symbols like #include

//compile all cpp files

//link all compiled cpp files together into one exe/program/application
int main() {
    const int size = 5;
    int myArray[size];
    int x;
    x = 3;
   
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

    //initialize an array
    //print array
    //find a value in the array   Cin<<value; 
    //if value found print index it was at if not print not found.

    //functions for each step.
    //function in a different file from the main function.
    return 0;
}