#include <iostream>
#include <vector>
#include <cmath>

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

bool tarkista_polku(vector<unsigned int> siirrot, vector<vector<char>>& g) {
    int x1 = siirrot[2]-1;
    int y1 = siirrot[3]-1;
    int x0 = siirrot[0]-1;
    int y0 = siirrot[1]-1;

    int distancey = y1-y0;
    int absy = abs(distancey);
    int distancex = x1-x0;
    int absx = abs(distancex);
    int risteysx = 1;
    int risteysy = 0;

    if (absy == 0) {
        while (x0 != x1) {
        if (g[y1][x1] == 'o') {
            if (x1 > x0)
                x1 -= 1;
            else if (x0 > x1)
                x1 += 1;
        } else if (g[y0][x0] == 'R' or g[y0][x0] == 'G') {
            cout << CANNOT_MOVE << endl;
            return false;
        }
        }
    } else {
        return true;
    }

    return true;
}

void move_piece(vector<unsigned int> siirrot, vector<vector<char>>& g) {
    vector<unsigned int> alku = {siirrot[0]-1, siirrot[1]-1};
    vector<unsigned int> loppu = {siirrot[2]-1, siirrot[3]-1};

    switch(g.at(loppu[1]).at(loppu[0]))
        {
        case ' ': cout << INVALID_POINT << endl; return;
        case 'R': cout << INVALID_POINT << endl; return;
        case 'G': cout << INVALID_POINT << endl; return;
        }

    switch(g.at(alku[1]).at(alku[0]))
        {
        case ' ': cout << INVALID_POINT << endl; return;
        case 'o': cout << INVALID_POINT << endl; return;
        case 'G': if (tarkista_polku(siirrot, g) == true) {
                    g.at(loppu[1]).at(loppu[0]) = 'G'; g.at(alku[1]).at(alku[0]) = 'o'; }; return;
        case 'R': if (tarkista_polku(siirrot, g) == true) {
                    g.at(loppu[1]).at(loppu[0]) = 'R'; g.at(alku[1]).at(alku[0]) = 'o'; }; return;
        }
    }


vector<vector<char>> initBoard() {
    vector<vector<char>> lauta = {{}, {}, {}, {}, {}};
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
        move_piece(siirrot, grid);
    }

    return 0;

}
