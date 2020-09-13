#include "player.hh"
#include <string>
#include <iostream>

using namespace std;

Player::Player(string nimi)
{
    nimi_ = nimi;
}

string Player::get_name()
{
    return nimi_;
}

int Player::get_points()
{
    return points_;
}

bool Player::has_won()
{
    if (points_ >= 50) {
        return true;
    } else {
        return false;
    }
}

void Player::add_points(int pojot)
{
    points_ += pojot;
    if (points_ > 50) {
        points_ = 25;
    }
}
