#include "ai.h"

AI::AI(std::vector<std::vector<char> > *b, int id_, std::string n) : Player::Player(b,  id_,  n)
{
    board.resize(X);
    for(int i = 0; i < X; i++)
    {
        board[i].resize(Y, -1);
    }
    copyBoard();
}

void AI::updateMoves()
{
    clearMoves();
    updateCloneableMoves();
    updateJumpMoves();
}

void AI::updateCloneableMoves()
{
    for(uint i = 0; i < Player::cloneable_sites.size(); i++)
    {
        Move m(Action::clone,Point(-1,-1),cloneable_sites[i]);
        moves.push_back(m);
        //qDebug() << "Clone Move added " << m.to.x << ","  << m.to.y;
    }
}


void AI::updateJumpMoves()
{
    /**
     * Reads jump_sites from the player
     * Generates move
     * Appends to list of moves
    **/
    for ( auto it = Player::jump_sites.begin(); it != Player::jump_sites.end(); ++it )
    {
        for(uint i = 0; i < it->second.size();i++)
        {
            Move m(jump,it->first,it->second[i]);
            moves.push_back(m);
            //qDebug() << "Jump Move added " << m.from.x << "," << m.from.y << "->" << m.to.x << m.to.y;
        }
    }
}

void AI::copyBoard()
{
    for(int  i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            board[i][j] = (*Player::board)[i][j];
        }
    }
    //qDebug() << "board copied";
}

void AI::dispMoves()
{
    for(uint i = 0; i < moves.size(); i++)
    {
        qDebug() << moves[i].action << moves[i].from.x << ","  << moves[i].from.y << " -> " << moves[i].to.x << "," << moves[i].to.y << " s:" << moves[i].score;
    }
}

int AI::playerType()
{
    return 1;
}

Move AI::getBestMove()
{
    simAllMoves();
    if(moves.size() > 0)
    {
        std::sort(moves.begin(),moves.end());
        int score = moves[0].score;
        auto it = moves.begin();
        for(; it != moves.end(); ++it)
        {
            Move m = *it;
            if(m.score < score)
            {
                moves.erase(it,moves.end());
                break;
            }
        }
        dispMoves();
        Move m = moves[0];
        if(moves.size() > 1)
            m = getRandomMove();
         qDebug() << "Best Move: "<< m.to.x << "," << m.to.y << " <- " << m.from.x << "," << m.from.y;
         return m;
     }
    else
    {
        qDebug() << "Boss, Game is Over";
        return Move(Action::invalid,Point(-1,-1),Point(-1,-1));
    }

}

Move AI::getRandomMove()
{
    int r_indx = rand() % (moves.size()-1);
    return moves[r_indx];
}

void AI::simAllMoves()
{
    copyBoard();
    updateMoves();
    for(uint i = 0; i < moves.size(); i++)
    {
           copyBoard();
           simMove(moves[i]);
    }
    //dispMoves();
}


void AI::clearMoves()
{
    moves.clear();
}

void AI::simMove(Move &m)
{

    int x = m.to.x;
    int y = m.to.y;

    board[x][y] = id;
    for(int  i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            int x_ = x + i;
            int y_ = y + j;
            if(x_ >= 0 && x_ < X && y_ >= 0 && y_ <Y)
            {
                if(board[x_][y_] != UNCLAIMED)
                {
                    board[x_][y_] = id;
                }
            }
        }
    }

    if(m.action == jump)
    {
        board[m.from.x][m.from.y] = UNCLAIMED;
    }
    m.score = getScore();
}

int AI::getScore()
{
    //qDebug() << "getting score";
    int score = 0;
    for(int i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            if(board[i][j] == id)
                score++;
        }
    }
    return score;
}
