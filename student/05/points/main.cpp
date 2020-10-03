#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

int main()
{
    string filename;
    cout << "Input file: ";
    getline(cin,filename);

    ifstream input_file(filename);
    map<string, int> scoreboard;
    if (not input_file) {
        cout << "Error! The file " << filename << " cannot be opened." << endl;
        return EXIT_FAILURE;
    } else {
        string rivi = "";
        while (getline(input_file, rivi)){
            int erotin = rivi.find(':');
            string name = rivi.substr(0,erotin);
            int pisteet = stoi(rivi.substr(erotin+1));
            if (scoreboard.find(name) == scoreboard.end() ) {
                scoreboard.insert({name, pisteet});
            } else {
                scoreboard.at(name) += pisteet;
            }
            }
        }
        cout << "Final scores:" << endl;
        for (auto tietopari : scoreboard) {
            cout << tietopari.first << ": " << tietopari.second << endl;
    }
}
