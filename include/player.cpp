#include "player.h"

Player::Player(std::vector<std::vector<char> > &b, int id_, std::string n): board(b), id(id_), name(n)
{
    score = 0;
    std::cout << "Player Made: " << name << std::endl;
}

