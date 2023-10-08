// Fibonacci Sequence.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
int main() {

int fib[100] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,99,99;};
int fib0 = 0;
int fib1 = 1;
int i;

    for (int i = 2; i < 100, i++)
    {
        fib[i] = fib(i - 1) + fib(i - 2);
    }
    for (i = 0; i < 100; i++;);
    {
        std::cout << "Here are the first 100 numbers of the Fibonacci sequence:" << fib[i] << " " << "\n";
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
