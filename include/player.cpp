#include "player.h"

Player::Player(std::vector<std::vector<char> > &b, int id_, std::string n): board(b), id(id_), name(n)
{
    Y = board.size();
    X = board[0].size();
    score = 0;
    std::cout << "Player Made: " << name << std::endl;
}

void Player::ownSites(const std::vector<Point> &indx)
{
    /** Claims ownership of the sites indexed  **/
    for(int i = 0; i < indx.size();i++)
    {
        board[indx[i].x][indx[i].y] = id;
        owned_sites.push_back(indx[i]);
    }
    score =owned_sites.size();
}

void Player::populateOwnedSites()
{
    /** Reads entire board - populates owned_sites vector and sets score
      * TODO : On fly computation - there are enough constraints that we
      * know this without looking at board.
    **/
    owned_sites.resize(0);
    for(int i = 0; i < X; i++)
    {
        for(int  j = 0; j < Y; j++)
        {
            if(board[i][j] == id)
            {
                owned_sites.push_back(Point(i,j));
            }
        }
    }
    score = owned_sites.size();
    qDebug() << QString::fromStdString(name) << " has " << QString(score) << "owned sites";
}

void Player::dispVector(std::vector<Player::Point> vec)
{
    qDebug() << QString::fromStdString(name) << ":";
    for(int i = 0; i < vec.size(); i++)
    {
        qDebug() << vec[i].x << "," << vec[i].y;
    }
}

void Player::eraseOwnedSites()
{
    owned_sites.resize(0);
    qDebug() << QString::fromStdString(name) << " owned sites erased :" << owned_sites.size();
}

std::vector<Player::Point> Player::getCloneableSites()
{
    std::vector<Player::Point> cloneable;
    for(int i = 0; i < owned_sites.size(); i++)
    {
        int x = owned_sites[i].x;
        int y = owned_sites[i].y;

        for(int i_ = -1; i_ <= 1; i_++)
        {
            for(int j_ = -1; j_ <=1; j_++)
            {
                int x_ = x + i_;
                int y_ = y + j_;
                if(x_ > -1 && x_ < X && y_>-1 && y_ < Y)
                {
                    if(Player::board[x_][y_] == UNCLAIMED)
                        cloneable.push_back(Player::Point(x_,y_));
                }
            }
        }

    }
    qDebug() << QString::fromStdString(name) <<"Displaying clonebale";
    return cloneable;
}
