#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/* Polku
 *
 * Kuvaus:
 * Ohjelma toteuttaa Polku-älypelin, jossa pelin tarkoituksena
 * on siirtää pelilaudan nappulat, ylhäällä vihreät (GREEN, 'G') ja alhaalla punaiset (RED, 'R'),
 * toisinpäin (vihreät alas & punaiset ylös). Nappulaa saa siirtää niin monta ruutua kuin
 * haluaa, kunhan siirto on laillinen: nappuloita ei saa liikuttaa toistensa yli eikä täten
 * ilman vapaista ruuduista (EMPTY, 'o') muodostuvaa polkua. Pelilaudan koko on COLUMS x ROWS (5x4).
 *
 * Pelaaminen tapahtuu antamalla ohjelmaan käskyn, joka koostuu lähtö- ja kohdepisteen
 * x- ja y-koordinaateista (neljä numeroa). Ohjelma sitten tarkistaa onko siirto
 * mahdollinen, jonka jälkeen pelaajalle tulostuu päivittynyt pelilauta laillisen
 * siirron jälkeen tai viesti siirron virheellisyydestä. Pelaaminen voidaan lopettaa
 * komennolla 'q'.
 *
 * Ohjelman kirjoittaja
 * Nimi: Mikko Perälä
 * Opiskelijanumero: 256960
 * Käyttäjätunnus: peralam
 * E-Mail: mikko.perala@tuni.fi
 * Palautteen kieli (fi/en): fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 *
 *
 * */



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
            case GREEN: cout << "G "; break;
            case RED: cout << "R "; break;
            case EMPTY: cout << "o "; break;
            case UNUSED: cout << "  "; break;
            }
        }
        cout << "|" << endl;
    }
    cout << "===============" << endl;
}

// Tarkistaa onko lähtö- ja kohdepisteen välillä vapaa polku
bool tarkista_polku(vector<unsigned int> siirrot, vector<vector<char>>& g) {
    int x1 = siirrot[2]-1;
    int y1 = siirrot[3]-1;
    int x0 = siirrot[0]-1;
    int y0 = siirrot[1]-1;

    int distancey = y1-y0;
    int absy = abs(distancey);

    if (absy == 0) {
        while (x0 != x1) {
        if (g[y1][x1] == EMPTY) {
            if (x1 > x0)
                x1 -= 1;
            else if (x0 > x1)
                x1 += 1;
        } else if (g[y0][x0] == RED or g[y0][x0] == GREEN) {
            cout << CANNOT_MOVE << endl;
            return false;
        }
        }
    } else {
            int risteys = 1;
            while (risteys != x0) {
                if (g[y0][risteys] == EMPTY) {
                    if (risteys > x0)
                        risteys -= 1;
                    else if (risteys < x0)
                        risteys += 1;
                } else if (g[y0][risteys] == RED or g[y0][risteys] == GREEN) {
                    cout << CANNOT_MOVE << endl;
                    return false;
                }
            }
        while (y0 != y1)  {
            if (g[y1][1] == EMPTY) {
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

// Liikuttaa pelinappulaa lähtöpisteestä kohdepisteeseen (jota ennen tarkistaa pisteiden välisen polun)
void move_piece(vector<unsigned int> siirrot, vector<vector<char>>& g, int& moves) {
    vector<unsigned int> alku = {siirrot[0]-1, siirrot[1]-1};
    vector<unsigned int> loppu = {siirrot[2]-1, siirrot[3]-1};

    switch(g.at(loppu[1]).at(loppu[0]))
        {
        case UNUSED: cout << INVALID_POINT << endl; return;
        case RED: cout << INVALID_POINT << endl; return;
        case GREEN: cout << INVALID_POINT << endl; return;
        }

    switch(g.at(alku[1]).at(alku[0]))
        {
        case UNUSED: cout << INVALID_POINT << endl; return;
        case EMPTY: cout << INVALID_POINT << endl; return;
        case GREEN: if (tarkista_polku(siirrot, g) == true) {
                    g.at(loppu[1]).at(loppu[0]) = GREEN;
                    g.at(alku[1]).at(alku[0]) = EMPTY;
                    print(g);
                    moves += 1;
                    }; return;
        case RED: if (tarkista_polku(siirrot, g) == true) {
                    g.at(loppu[1]).at(loppu[0]) = RED;
                    g.at(alku[1]).at(alku[0]) = EMPTY;
                    print(g);
                    moves += 1;
                    }; return;
        }
    }
// Tarkistaa onko peli voitettu (kaikki ylärivillä punaista ja alarivillä vihreää)
bool gameWon(vector<vector<char>>& g) {
    for (unsigned int i = 0; i < 4; i++) {
        if (g[0][i] == RED and  g[4][i] == GREEN) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}
// Alustaa pelilaudan mallin mukaisesti
vector<vector<char>> initBoard() {
    vector<vector<char>> lauta = {{}, {}, {}, {}, {}};
    lauta.at(0) = {GREEN, GREEN, GREEN, GREEN};
    lauta.at(1) = {UNUSED, EMPTY, UNUSED, UNUSED};
    lauta.at(2) = {UNUSED, EMPTY, EMPTY, UNUSED};
    lauta.at(3) = lauta.at(1);
    lauta.at(4) = {RED, RED, RED, RED};
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
// Erottelee käyttäjän antamasta syötteestä lähtö- ja kohdepisteen koordinaatit (4 numeroa)
// Jos syöte on virheellinen, keskeyttää funktion ja palaa takaisin mainin alkuun virhesanoman saattelemana
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

// Alustaa ja tulostaa pelilaudan, jonka jälkeen ottaa vastaan
// pelaajalta syötteen (siirto tai lopetus)
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
