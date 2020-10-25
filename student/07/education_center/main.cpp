#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

// Virhesanomat, joita kutsutaan täältä myöhemmin tarvittaessa.
string READ_ERROR = "Error: the input file cannot be opened";
string FILE_ERROR = "Error: empty field";
string UNKNOWNCOMMAND = "Error: Unknown command: ";
string INVALIDPARAMETERS = "Error: error in command ";
string INVALIDLOCATION = "Error: unknown location name";
string INVALIDTHEME = "Error: unknown theme";

// Kursseissa käytettävä rakenne, jonka parametreinä toimivat kurssin teema, nimi ja osallistujamäärä.
struct Course {
    string theme;
    string name;
    int enrollments;

};

/* Tekstirivien (location, theme, name, enrollments) ja syötekomentojen splittaaminen osiin,
* jotka palautetaan mainiin vektorina.
* Etsii tekstiriviltä annetun erotinmerkin ja erottelee sanat niiden väliltä.
* Huomioi lainausmerkeissä olevat tekstit (sekä tiedostoissa että syötteissä) ja sijoittaa ne
* palautettavaan vektoriin ilman lainausmerkkejä.
*/
vector<string> split (const string& r, const char separator, bool includeSpaces = false) {
    string temp = r;
    vector<string> parts;

    while (temp.find(separator) != string::npos) {
        int stop = temp.find(separator);
        string new_part = temp.substr(0,stop);
        // Sallii osien välillä olevat lainausmerkit, mikäli sana on lainausmerkeissä.
        // Uusi katkaistava kohta (jälkimmäinen lainausmerkki) etsitään sanan lopusta alkaen.
        if (includeSpaces == true and new_part[0] == '"') {
            stop = temp.rfind('"');
            new_part = temp.substr(1, stop-1);
        }
        temp = temp.substr(stop+1, temp.size()-1);
        parts.push_back(new_part);
        stop = temp.find(stop);

    }  
    // Kurssi on täynnä mikäli sillä on 50 osallistujaa. Muutetaan numeeriseksi arvoksi
    // laskutoimitusten helpottamiseksi.
    if (temp == "full") {
        temp = "50"; }
    if (temp != "") {
        parts.push_back(temp);
    }

    // Tarkistaa, ettei syötteisiin tai tietorakenteisiin menevissä arvoissa ole lainausmerkkejä.
    for (auto& i : parts) {
        int lastLetter = i.length()-1;
        if (i.at(0) == '"' and i.at(lastLetter) == '"') {
            i = i.substr(1, lastLetter-1);
        }
    }
    return parts;
}

/* Järjestää vektorissa olevat parit arvojen mukaan laskevaan järjestykseen.
 * Käytetään suosituimpien kurssien selvittämiseksi.
*/
bool sortByValue(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}
/* Järjestää structit aakkosjärjestykseen ensisijaisesti kurssin teeman mukaan ja
 * muussa tapauksessa kurssin nimen mukaan.
*/
bool sortAlphabetically(const Course& c1, const Course& c2) {
    if (c1.theme == c2.theme) {
        return c1.name < c2.name;
    } else {
        return c1.theme < c2.theme;
    }
}

/* Järjestää ohjelman tietorakenteesta löytyvät kurssien teemat ja niitä vastaavat
 * kokonaisosallistujamäärät pareiksi vektoriin. Tietoparit järjestetään osallistujamäärien
 * mukaan kutsumalla vertailufunktiota ja palautetaan mainiin, josta funktiota kutsuttiin.
*/
vector<pair<string, int>> find_favorite(map<string, vector<Course>> d) {
    map<string, int> courseEnrolls = {};
    vector<pair<string, int>> topCourses = {};
    for (auto course : d) {
        for (auto it : course.second) {
            string theme = it.theme;
            courseEnrolls[theme] += it.enrollments;
        }
    }
    for (auto pair : courseEnrolls) {
        topCourses.push_back(pair);
    }
    sort(topCourses.begin(),topCourses.end(),sortByValue);

    return topCourses;
}

/* Alustaa tietorakenteen (structit vektoriin ja vektorit mappiin, avaimina kurssin paikkakunta).
 * Järjestää vektorissa olevat kurssitiedot aakkoselliseen järjestykseen kurssien teemojen ja nimien mukaisesti
 * kutsumalla vertailufunktiota.
 * Mikäli tiedostossa on tyhjiä kohtia tai tiedostoa ei saada avattua, tulee ikkunaan virheviesti ja ohjelma loppuu.
*/

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
        // Tiedoston rivien lukeminen ja jaottelu osiin (katso: split-funktio)
        string rivi;
        while (getline(read_file,rivi)) {
            vector<string> parts = split(rivi, ';');
            for (auto i : parts) {
                if (i == "" or i == " ") {
                    cout << FILE_ERROR << endl;
                    return EXIT_FAILURE;
                }
            }
            // Kurssin rakenteen (teema, nimi, osallistujat) tallennus mappiin (paikkakunnan arvoksi).
            // Mikäli kurssi samalla nimellä ja teemalla löytyy jo (same course), tallettaa uusimman tiedon osallistujamäärästä.

            struct Course coursetemp = {parts[1], parts[2], stoi(parts[3])};
            map<string, vector<Course>>::iterator it = database.find(parts[0]);
            if (it != database.end()) {
                bool sameCourse = false;
                for (auto& kurssi : database.at(parts[0])) {
                    if (kurssi.name == parts[2]) {
                        sameCourse = true;
                     kurssi.enrollments = stoi(parts[3]);
                    }
                } if (sameCourse == true) {
                    continue;
                } database.at(parts[0]).push_back(coursetemp);
            } else {
                database.insert({parts[0], {coursetemp}});
            }

        } read_file.close();

        // Järjestää tietorakenteessa olevat kurssitiedot aakkosjärjestykseen (katso: sortAlphabetically -funktio)
        for (auto& course : database) {
            sort(course.second.begin(), course.second.end(), sortAlphabetically);
        }

        // Ottaa vastaan käyttäjän syötteitä (komentoja) ja erottelee osat split-funktion avulla,
        // kunnes käyttäjä antaa komennon "quit".
    while(true) {
        vector<string> commands;
        string input;
        cout << "> ";
        getline(cin, input);
        commands = split(input, ' ', true);

        // Tulostaa kurssikeskuksessa olevien paikkakuntien nimet (mapin avaimet).
        if (commands[0] == "locations") {
            for (auto it : database) {
                cout << it.first << endl;
            }

        /* Ottaa vastaan vain 3-osaisen komennon (courses, paikkakunta, teema) ja antaa virhesanoman,
         * mikäli paikkakuntaa ei tunnisteta, kurssia ei tunnisteta tai parametrien määrä on väärä.
         * Etsii kurssitarjooman käyttäjän määrittelemältä paikkakunnalta ja halutusta teemasta,
         * ja tulostaa kurssin nimen sekä osallistujamäärän. Mikäli kurssi on täynnä, funktio
         * ilmoittaa osallistujamääräksi "full".
         */

        } else if (commands[0] == "courses") {
            if (commands.size() != 3) {
                cout <<  INVALIDPARAMETERS << commands[0] << endl;
            } else if (database.find(commands[1]) == database.end()) {
                cout << INVALIDLOCATION << endl;
            } else {
                map<string, vector<Course>>::iterator iter = database.find(commands[1]);
                bool knownCourse = false;
                    for (auto it : iter->second) {
                        if (it.theme == commands[2]) {
                            knownCourse = true;
                            if (it.enrollments == 50) {
                                cout << it.name << " --- " << "full" << endl;
                            } else {
                            cout << it.name << " --- " << it.enrollments << " enrollments" << endl;
                            }
                        }
                    }
                    // Virhesanoma mikäli käyttäjä syöttää teeman, jota kurssikeskuksesta ei löydy.
                    if (knownCourse == false) {
                        cout << INVALIDTHEME << endl;
                    }
            }
        // Ottaa vastaan 2 parametria: paikkakunta ja teema.
        // Etsii tietorakenteesta kaikilta paikkakunnilta toivottuun teemaan sisältyvät kurssit.
        // Antaa virhesanoman, mikäli käyttäjän antamasta teemasta ei löydy kursseja.
        } else if (commands[0] == "courses_in_theme") {
            set<string> courseNames = {};
            for (auto kurssi: database) {
                for (auto it:kurssi.second) {
                    if (it.theme == commands[1]) {
                        courseNames.insert(it.name);
                    }
                }
            }
            if (courseNames.size() == 0) {
                cout << INVALIDTHEME << endl;
            } else {
                for (auto i : courseNames) {
                    cout << i << endl;
                }
            }
        /* Etsii kurssikeskuksesta suosituimmat kurssiteemat.
         * Tulostaa suosituimman kurssin (tai kurssien) teeman ja kokonaisosallistujamäärän kaikilta paikkakunnilta.
         * Mikäli kurssitarjonnan tiedosto on tyhjä tai ilmoittautumisia ei ole, tulostaa ilmoituksen "Ei ilmoittautumisia".
        */
        } else if (commands[0] == "favorite_theme") {
            vector<pair<string, int>> topCourses = find_favorite(database);
            int maxEnrolls = 0;
            for (auto i : topCourses) {
                if (i.second > maxEnrolls) {
                    maxEnrolls = i.second;
                }
            }
            if (maxEnrolls == 0) {
                cout << "No enrollments" << endl;
            } else {
                cout << maxEnrolls << " enrollments in themes" << endl;
                for (auto i : topCourses)
                    if (i.second == maxEnrolls) {
                        cout << "--- " << i.first << endl;
                }
            }

        // Tulostaa kaikki kurssikeskuksen kurssit, jotka eivät ole vielä täynnä (osallistujamäärä alle 50).
        } else if (commands[0] == "available") {
            for (auto course : database) {
                for (auto it : course.second) {
                    if (it.enrollments != 50) {
                        cout << course.first << " : " << it.theme << " : " << it.name << endl;
                    }
                }
            }
        // Sulkee ohjelman.
        } else if (commands[0] == "quit") {
                   return EXIT_SUCCESS;

        // Antaa virheviestin tuntemattomasta komennosta.
        } else {
            cout << UNKNOWNCOMMAND << commands[0] << endl;
        }
    }
    }
}
