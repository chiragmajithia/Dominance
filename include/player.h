#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <QDebug>
#include <unordered_map>
#include "point.h"

#define UNCLAIMED -1

using namespace type;
class Player
{
public:

    Player(): board(NULL){}

    Player(std::vector<std::vector<char> > *b, int id_, std::string n);

public:
    std::vector<std::vector<char> > *board;
    std::string name;
    int id, score;
    int X, Y;


    int available_moves;
    std::vector<Point> owned_sites;
    std::vector<Point> cloneable_sites;
    std::unordered_map< Point, std::vector<Point>, KeyHasher > jump_sites;

    void ownSites(const std::vector<Point> &indx);
    void populateOwnedSites();
    std::vector<Point> getCloneableSites();
    void eraseOwnedSites();
    void setCloneableSites();
    void setJumpSites();
    std::vector<Point> getJumpSites(Point p);
    void updateScore();
    uint getAvailableMoves();
    virtual int playerType();

    /** Debug Helper Functions **/
    void dispBoard();
    void dispVector(std::vector<Point> vec);

};

#endif // PLAYER_H
