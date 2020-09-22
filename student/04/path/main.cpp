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
            int risteys = 1;
            while (risteys != x0) {
                if (g[y0][risteys] == 'o') {
                    if (risteys > x0)
                        risteys -= 1;
                    else if (risteys < x0)
                        risteys += 1;
                } else if (g[y0][risteys] == 'R' or g[y0][risteys] == 'G') {
                    cout << CANNOT_MOVE << endl;
                    return false;
                }
            }
        while (y0 != y1)  {
            if (g[y1][1] == 'o') {
                if (y1 > y0)
                    y1 -= 1;
                else if (y1 < y0)
                    y1 += 1;
            } else {
                cout << CANNOT_MOVE << endl;
                return false;
            }
        }
        return true;
    }
    return true;
}

void move_piece(vector<unsigned int> siirrot, vector<vector<char>>& g, int& moves) {
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
                    g.at(loppu[1]).at(loppu[0]) = 'G';
                    g.at(alku[1]).at(alku[0]) = 'o';
                    print(g);
                    moves += 1;
                    }; return;
        case 'R': if (tarkista_polku(siirrot, g) == true) {
                    g.at(loppu[1]).at(loppu[0]) = 'R';
                    g.at(alku[1]).at(alku[0]) = 'o';
                    print(g);
                    moves += 1;
                    }; return;
        }
    }
bool gameWon(vector<vector<char>>& g) {
    for (unsigned int i = 0; i < 4; i++) {
        if (g[0][i] == 'R' and  g[4][i] == 'G') {
            continue;
        } else {
            return false;
        }
    }
    return true;
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
            if (move.length() > 1) {
                break;
            }
            unsigned int siirto = stoi_with_check(move);
            if (siirto == 0)
                break;
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
    int moves = 0;
    print(grid);
    while (gameWon(grid) == false) {
        cout << INPUT_TEXT;
        getline(cin, komento);
        if (komento != "q") {
            vector<unsigned int> siirrot = split(komento);
            if (siirrot.size() != 4) {
                cout << INVALID_POINT << endl;
                continue;
            } else {
                move_piece(siirrot, grid, moves);
            }
        } else {
            cout << moves << MOVES_MADE << endl;
            return EXIT_SUCCESS;
        }
    }
        cout << GAME_OVER << endl;
        cout << moves << MOVES_MADE << endl;
        return EXIT_SUCCESS;
}
