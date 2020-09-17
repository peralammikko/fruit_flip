#include <iostream>
#include <vector>

using namespace std;

enum Slot_type {RED, GREEN, EMPTY, UNUSED};
const unsigned int ROWS = 5;
const unsigned int COLUMS = 4;

const string INPUT_TEXT = "Enter start point (x, y) and destination point (x, y), or q to quit: ";
const string INVALID_POINT = "Invalid start/destination point.";
const string CANNOT_MOVE = "There is no path between start point and destination point.";
const string GAME_OVER = "Game over. Congratulations!";
const string MOVES_MADE = " move(s) made.";

// Prints the grid
void print(const vector<vector<char>>& g)
{
    cout << "===============" << endl;
    cout << "|   | 1 2 3 4 |" << endl;
    cout << "---------------" << endl;
    for(unsigned int i = 0; i < ROWS; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < COLUMS; ++j)
        {
            switch(g.at(i).at(j))
            {
            case 'G': cout << "G "; break;
            case 'R': cout << "R "; break;
            case 'o': cout << "o "; break;
            case ' ': cout << "  "; break;
            }
        }
        cout << "|" << endl;
    }
    cout << "===============" << endl;
}

vector<vector<char>> initBoard() {
    vector<vector<char>> lauta = {{}, {}, {}, {}, {}, {}};
    lauta.at(0) = {'G', 'G', 'G', 'G'};
    lauta.at(1) = {' ', 'o', ' ', ' '};
    lauta.at(2) = {' ', 'o', 'o', ' '};
    lauta.at(3) = lauta.at(1);
    lauta.at(4) = {'R', 'R', 'R', 'R'};
    return lauta;
}

// Converts the given numeric string to the corresponding integer
// (by calling stoi).
// If the given string is not numeric, returns 0.
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

vector<unsigned int> split(string komento) {
    vector<unsigned int> merkit = {};
    int pituus = komento.length();
    int i = 0;
    string move = "";
    char tyhja = ' ';
    while (i < pituus) {
        char merkki = komento.at(i);

        if (merkki != tyhja) {
            move += merkki;
            unsigned int siirto = stoi_with_check(move);
            if (i==pituus-1)
                merkit.push_back(siirto);
        } else if (merkki == tyhja) {
            unsigned int siirto = stoi_with_check(move);
            merkit.push_back(siirto);
            move = "";
        }
        i++;
    }
    return merkit;
}

// More functions

int main()
{
    // More code
    vector<vector<char>> grid = initBoard();
    string komento = "";
    while (komento != "q") {
        print(grid);
        cout << INPUT_TEXT;
        getline(cin, komento);
        vector<unsigned int> siirrot = split(komento);
    }
    return 0;

}
