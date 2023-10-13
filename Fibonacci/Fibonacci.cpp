// Fibonacci 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


int main() {
    const int size = 100;
    long long fibonacci[size];

    // Initialize the first two Fibonacci numbers
    fibonacci[0] = 0;
    fibonacci[1] = 1;

    // Generate the Fibonacci sequence
    for (int i = 2; i < size; ++i) {
        fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
    }

    // Print the Fibonacci sequence
    for (int i = 0; i < size; ++i) {
        std::cout << fibonacci[i] << " ";
    }

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
