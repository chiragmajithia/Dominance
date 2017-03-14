#ifndef AI_H
#define AI_H

#include <unordered_map>
#include <cstdlib>
#include <vector>
#include <QDebug>
#include "player.h"
#include  "point.h"

#define UNCLAIMED -1

using namespace type;

class AI: public Player
{
public:
    AI();
    AI(std::vector<std::vector<char> > *b, int id_, std::string n);



    std::vector< Move > moves;
    std::vector< Move > best_moves;
    std::vector< std::vector<char> > board; //own copy of board


    Move getBestMove();
    void dispMoves();
    void copyBoard();
    int playerType();

private :
    void updateMoves();
    void updateCloneableMoves();
    void updateJumpMoves();
    void clearMoves();
    void simMove(Move &m);
    int getScore();
    void simAllMoves();
    Move getRandomMove();
};

#endif // AI_H
