#include <iostream>
#include <time.h>

int main()
{
    int x;
    //scope 
    if (true)
    {
        x = 3;
        x = 4;
        x = x + 5;
        //...
    }
    else
    {
        x = 6;
        int y = 4;
        y = 5;
        y = y + 2;
        //...
    }
    x = 3;
    //loops
        //while
        //do while
        //for
    float tempF = -100;
    do
    {
        std::cout << "temp F please: (-100 to stop)";
        std::cin >> tempF;

        float tempC = tempF - 32 * (5 / 9.0f);
        std::cout << "your temp in C is " << tempC << std::endl;

    } while (tempF != -100);
    
    //random numbers
    srand(time(NULL));
    std::cout << "arandom number " << ((float)rand() / RAND_MAX)*10 <<"\n";

    //array
    int employeeAge1 = 19;
    int employeeAge2 = 29;
    int employeeAge3 = 39;

    (employeeAge1 + employeeAge2 + employeeAge3) / 3;

    int employeeAge[10] = { 0,1,2,3,4,5,6,7,8,9 };//0...9

    //employeeAge[0] + employeeAge[1] + employeeAge[2] + employeeAge[3] + employeeAge[4] +

    int total=0;
    for (int i = 0; i < 10; i++)//0...9
    {
        total+=employeeAge[i];
    }

    std::cout<<"avg "<<total / 10<<"\n";

    //for each
    total = 0;
    for (int element: employeeAge)//0...9
    {
        total += element;
    }

    float myFloat;
    float myFloat2[10];
    std::string names[10];
    std::cout << "avg2 " << total / 10 << "\n";

    //create an array of size 100, filled with the fibinochi sequence
    //0,1,1,2,3,5,8,13...
    return 0;

}