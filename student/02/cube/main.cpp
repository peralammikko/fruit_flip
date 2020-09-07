#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    int luku = 0;
    int kuutio = 0;
    int max = 2147483647;
    cout << "Enter a number: ";
    cin >> luku;

    kuutio = pow(luku,3);
    if (kuutio > max || kuutio < 0 || kuutio < luku) {
        cout << "Error! The cube of " << luku << " is not " << kuutio << "." << endl;
    }
    else {
        cout << "The cube of number " << luku << " is " << kuutio << "." << endl;

}
    return 0;
}
