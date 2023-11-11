// Function Homework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i]; int j = i - 1; while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void OriginalArray()
{
    int nums[5];

    for (int i = 0; i < 5; i++)
    {
        nums[i] = 5 - i;
        std::cout << nums[i] << "\n";
    }
}

void ArrangeArray()
{
    const int SIZE = 5;
    int arr[SIZE] = {5, 4, 3, 2, 1};

    for (int i = 0; i < SIZE-1; i++)
    {
        for (int j = 0; j < SIZE-i-1; j++) {
            if (arr[j] > arr[j+1]) 
            {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    // print sorted array
    for (int i = 0; i < SIZE; i++)
    {
        std::cout << arr[i] << " ";
    }
}

int main()
{
    OriginalArray();
    ArrangeArray();

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
