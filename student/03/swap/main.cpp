#include <cstdlib>
#include <iostream>

// Write your swap function here.
void swap(int& i, int& j) {
    int temp = 0;
    temp = i;
    i = j;
    j = temp;
}


#ifndef UNIT_TESTING
int main()
{
    std::cout << "Enter an integer: ";
    int i = 0;
    std::cin >> i;

    std::cout << "Enter another integer: ";
    int j = 0;
    std::cin >> j;

    swap(i, j);
    std::cout << "The integers are " << i << " and " << j << std::endl;

    return EXIT_SUCCESS;
}
#endif
