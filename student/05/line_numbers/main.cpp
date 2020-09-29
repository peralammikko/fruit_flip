#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    string filename = "";
    cout << "Input file: ";
    getline(cin, filename);

    string output = "";
    cout << "Output file: ";
    getline(cin, output);

    ifstream input_olio(filename);
    if (not input_olio) {
        cout << "Error! The file " << filename <<" cannot be opened." << endl;
    } else {
        ofstream output_olio(output);
        string rivi;
        int i = 1;
        while (getline(input_olio, rivi)) {
            output_olio << i << " " << rivi << endl;
            i++;
        }
        input_olio.close();
    }
}
