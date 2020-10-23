#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

string READ_ERROR = "Error: the input file cannot be opened";
string FILE_ERROR = "Error: empty field";
string UNKNOWNCOMMAND = "Error: unknown command: ";
string INVALIDPARAMETERS = "Error: error in command ";
string INVALIDLOCATION = "Error: unknown location name";
string INVALIDTHEME = "Error: Unknown theme";

struct Course {
    string theme;
    string name;
    int enrollments;

    bool operator<(const Course& c1) const {
        return theme < c1.theme;
    }


};

// Tekstirivien splittausosiin (location, theme, name, enrollments), jotka palautetaan mainiin.
vector<string> split (const string& r, const char erotin) {
    string temp = r;
    vector<string> osat;

    while (temp.find(erotin) != string::npos) {
        int erotus = temp.find(erotin);
        string new_part = temp.substr(0,erotus);
        temp = temp.substr(erotus+1, temp.size()-1);
        osat.push_back(new_part);
    }
    if (temp == "full")
        temp = "50";
    osat.push_back(temp);
    return osat;
}

int find_favorite(map<string, set<Course>>& d) {
    return 0;
}


int main()
{

    map<string, set<Course>> database;
    string file = "";
    cout << "Input file: ";
    getline(cin, file);

    // Tiedoston lukeminen ja virhetilanteiden viestit (tiedostoa ei ole / tyhjiä kenttiä)
    ifstream read_file(file);
    if (not read_file) {
        cout << READ_ERROR << endl;
        return EXIT_FAILURE;
    } else {
        string rivi;
        while (getline(read_file,rivi)) {
            vector<string> parts = split(rivi, ';');
            for (auto i : parts) {
                if (i == "" or i == " ") {
                    cout << READ_ERROR << endl;
                    return EXIT_FAILURE;
                }
            }
            // Kurssin rakenteen tallennus mappiin
            struct Course coursetemp = {parts[1], parts[2], stoi(parts[3])};
            if (database.find(parts[0]) != database.end()) {
                map<string, set<Course>>::iterator it = database.find(parts[0]);
                for (auto c : it->second) {
                    if (coursetemp.theme == c.theme) {
                        if (coursetemp.name == c.name) {
                            database[parts[0]] = {coursetemp};
                        } else {
                            database[parts[0]].insert({coursetemp});
                        }
                    } else {
                        database[parts[0]].insert({coursetemp}); }
                }
            } else {
                database[parts[0]].insert({coursetemp});
            }
        }
        for (auto kurssi : database) {
            cout << kurssi.first << ": " << endl;
            for (auto tieto : kurssi.second){
                cout << tieto.theme << endl;
                cout << tieto.name << endl;
                cout << tieto.enrollments << endl;
                cout << endl;
            }
        }
        read_file.close();

    while(true) {
        vector<string> commands;
        string input;
        cout << "> ";
        getline(cin, input);
        commands = split(input, ' ');

        if (commands[0] == "locations") {
            for (auto it : database) {
                cout << it.first << endl;
            }
        } else if (commands[0] == "courses") {
            if (commands.size() != 3) {
                cout <<  INVALIDPARAMETERS << commands[0] << endl;
            } else if (database.find(commands[1]) == database.end()) {
                cout << INVALIDLOCATION << endl;
            }
        } else if (commands[0] == "favorite_theme") {
            cout << find_favorite(database) << " enrollments" << endl;

        } else if (commands[0] == "courses_in_theme") {
            for (auto kurssi: database) {
                for (auto it:kurssi.second) {
                    if (it.theme == commands[1]) {
                        cout << it.name << endl;
                    }
                }
            }

        } else if (commands[0] == "available") {
            for (auto kurssi : database) {
                for (auto it : kurssi.second) {
                    if (it.enrollments != 50) {
                        cout << kurssi.first << " : ";
                        cout << it.theme << " : ";
                        cout << it.name << endl;
                    }
                }
            }

        } else if (commands[0] == "quit") {
                   return EXIT_SUCCESS;
    }
    }
    }
}
