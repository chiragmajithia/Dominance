#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <string>
#include <iostream>

class Player
{
public:
    Player(){};
    Player(std::vector<std::vector<char> > &b, int id_, std::string n);
    struct Point{
        int x,y;
        Point():x(0),y(0){}
    };

public:
    std::vector<std::vector<char> > board;
    std::string name;
    int id, score;

    std::vector<Point> owned_sites;

};

#endif // PLAYER_H
