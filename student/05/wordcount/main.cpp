#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>

using namespace std;

int main()
{
    string filename;
    cout << "Input file: ";
    getline(cin,filename);

    ifstream input_file(filename);
    map<string, set<int>> lista;
    if (not input_file) {
        cout << "Error! The file " << filename << " cannot be opened." << endl;
        return EXIT_FAILURE;
    } else {
        string rivi = "";
        char erotin = ' ';
        int rivinro = 1;
        while (getline(input_file, rivi)) {
            int i = 0;
            string sana;
            int pituus = rivi.length();
            while (i != pituus) {
                char merkki = rivi.at(i);
                if (merkki == erotin) {
                    lista[sana].insert(rivinro);
                    i++;
                    sana = "";
                } else if (merkki != erotin) {
                    sana += merkki;
                    if (i == pituus-1) {
                        lista[sana].insert(rivinro);
                    }
                    i++;
                }
            }
            rivinro++;
        }
        for (auto tietopari:lista) {
            cout << tietopari.first << " " << tietopari.second.size() << ": ";
            unsigned int i = 0;
            for (auto numero : tietopari.second) {
                i++;
                if (i==tietopari.second.size())
                    cout << numero;
                else
                    cout << numero << ", ";
            }
            cout << endl;
        }
    }

}
