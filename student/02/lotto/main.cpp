#include <iostream>

using namespace std;

unsigned long int fact(int pallot) {
    unsigned long int kertoma = 1;
    for (int i = 1; i <= pallot; i++) {
        kertoma = i*kertoma;
    }
    return kertoma;
}

int main()
{
    int balls = 0;
    int draws = 0;
    cout << "Enter the total number of lottery balls: ";
    cin >> balls;
    cout << "Enter the number of drawn balls: ";
    cin >> draws;

    if (balls < 0 || draws < 0) {
        cout << "The number of balls must be a positive number." << endl;
    } else if (draws > balls) {
        cout << "The maximum number of drawn balls is the total amount of balls." << endl;
    } else {
        int loput = balls-draws;
        unsigned long int factballs = fact(balls);
        unsigned long int factdraws = fact(draws);
        unsigned long int factloput = fact(loput);
        unsigned long int prob = (factballs)/(factloput*factdraws);
        cout << "The probability of guessing all "<< draws << " balls correctly is 1/" << prob << endl;
    }
    return 0;
}
