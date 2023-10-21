
#include <iostream>

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

void sortingArray(int arr[], int size)
{

}