#ifndef PLAYER_HH
#define PLAYER_HH
#include <string>

using namespace std;


class Player
{
public:
    Player();
    Player(string nimi_);
    string get_name();
    int get_points();
    void add_points(int pojot);
    bool has_won();

private:
    string nimi_;
    int points_ = 0;
};

#endif // PLAYER_HH
