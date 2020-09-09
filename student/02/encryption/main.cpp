#include <iostream>
#include <string>

using namespace std;

string abc = "abcdefghijklmnopqrstuvwxyz";

bool check(string key) {
    int pituus = key.length();
    if (pituus < 26) {
        cout << "Error! The encryption key must contain 26 characters." << endl;
        return false;
    }
    for (int i = 0; i < pituus; i++) {
        int merkki = static_cast<int>(key.at(i));
        if (merkki < 97 || merkki > 122) {
            cout << "Error! The encryption key must contain only lower case characters." << endl;
            return false;
            }
        }
    for (int i = 0; i < pituus; i++) {
        string::size_type kirjain = key.find(abc.at(i));
        if (kirjain == string::npos) {
            cout << "Error! The encryption key must contain all alphabets a-z." << endl;
            return false;
        }
    }
    return true;
}

string encrypt(string key, string text) {
    string viesti = "";
    int vpituus = text.length();

    for (int i = 0; i < vpituus; i++) {
        string::size_type indeksi = abc.find(text.at(i));
        char merkki = key.at(indeksi);
        viesti = viesti + merkki;
    }
    return viesti;
}

int main()
{
    string key = "";
    string text = "";
    cout << "Enter the encryption key: ";
    cin >> key;
    if (check(key) == true) {
        cout << "Enter the text to be encrypted: ";
        cin >> text;
        string salaviesti = encrypt(key,text);
        cout << "Encrypted text: " << salaviesti << endl;
    } else {
        return EXIT_FAILURE;
    }

}
