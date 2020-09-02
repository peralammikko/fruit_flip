#include <iostream>

using namespace std;


int main()
{
    int luku = 0;
    std::cout << "How many numbers would you like to have? ";
    cin >> luku;

    for (int i = 1; i <= luku; ++i) {
        cout << i << endl;
    }
}
