#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

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

bool sortByValue(const pair<string, int> course1, const pair<string, int> course2) {
    return course1.second < course2.second;
}

bool sortAlphabetically(const Course& c1, const Course& c2) {
    if (c1.theme == c2.theme) {
        return c1.name < c2.name;
    } else {
        return c1.theme < c2.theme;
    }
}

map<string, int> find_favorite(map<string, vector<Course>> d) {
    map<string, int> courses = {};
    map<string, vector<Course>>::iterator iter;
    for (auto kurssi : d) {
        for (auto it : kurssi.second) {
            int tempEnrolls = 0;
            string theme = it.theme;
            tempEnrolls += it.enrollments;
            courses[theme] += tempEnrolls;
        }
    }
    return courses;
}



int main()
{

    map<string, vector<Course>> database;
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
                    cout << FILE_ERROR << endl;
                    return EXIT_FAILURE;
                }
            }
            // Kurssin rakenteen tallennus mappiin
            struct Course coursetemp = {parts[1], parts[2], stoi(parts[3])};
            map<string, vector<Course>>::iterator it = database.find(parts[0]);
            if (it != database.end()) {
                bool samakurssi = false;
                for (auto& kurssi : database.at(parts[0])) {
                    if (kurssi.name == parts[2]) {
                        samakurssi = true;
                     kurssi.enrollments = stoi(parts[3]);
                    }
                } if (samakurssi == true) {
                    continue;
                } database.at(parts[0]).push_back(coursetemp);
            } else {
                database.insert({parts[0], {coursetemp}});
            }

        } read_file.close();

        for (auto& kurssi : database) {
            sort(kurssi.second.begin(), kurssi.second.end(), sortAlphabetically);
            cout << kurssi.first << ": " << endl;
            for (auto tieto : kurssi.second){
                cout << tieto.theme << endl;
                cout << "- " << tieto.name << endl;
                cout << "- " << tieto.enrollments << endl;
            }
            cout << endl;
        }

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

        } else if (commands[0] == "courses_in_theme") {
            set<string> courseNames = {};
            for (auto kurssi: database) {
                for (auto it:kurssi.second) {
                    if (it.theme == commands[1]) {
                        courseNames.insert(it.name);
                    }
                }
            }
            for (auto i : courseNames) {
                cout << i << endl;
            }

        } else if (commands[0] == "favorite_theme") {
            map<string, int> courseEnrolls = find_favorite(database);
            int maxEnrolls = 0;
            string favorite = "";
            for (auto i : courseEnrolls) {
                if (i.second > maxEnrolls) {
                    maxEnrolls = i.second;
                }
                cout << i.first << " : " << i.second << endl;
            }


        } else if (commands[0] == "available") {
            for (auto kurssi : database) {
                for (auto it : kurssi.second) {
                    if (it.enrollments != 50) {
                        cout << kurssi.first << " : " << it.theme << " : " << it.name << endl;
                    }

                }
            }

        } else if (commands[0] == "quit") {
                   return EXIT_SUCCESS;
    }
    }
    }
}