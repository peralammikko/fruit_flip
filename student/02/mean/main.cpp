#include <iostream>

using namespace std;
// Write here a function counting the mean value

int main()
{
    int luku;
    int summa = 0;
    int nro = 0;
    std::cout << "From how many integer numbers you want to count the mean value? ";
    cin >> luku;
    if (luku <= 0) {
        cout << "Cannot count mean value from " << luku << " numbers." << endl;
        return 0;
    }

    for (int i = 1; i<=luku; i++) {
        cout << "Input " << i << ". number: ";
        cin >> nro;
        summa = summa+nro;
        nro = 0;
    }
    cout << "Mean value of the given numbers is " << summa/luku << endl;
    return 0;
}
