#include "player.h"

int Player::playerType()
{
    return 0;
}

Player::Player(std::vector<std::vector<char> > *b, int id_, std::string n): board(b), id(id_), name(n)
{
    X = (*board).size();
    Y = (*board)[0].size();
    score = 0;
    available_moves = 0;
    std::cout << "Player Made: " << name << std::endl;
}

void Player::ownSites(const std::vector<Point> &indx)
{
    /** Claims ownership of the sites indexed  **/
    for(uint i = 0; i < indx.size();i++)
    {
        (*board)[indx[i].x][indx[i].y] = id;
        owned_sites.push_back(indx[i]);
    }
    score =owned_sites.size();
}

void Player::updateScore()
{
    score = owned_sites.size();
}

void Player::setJumpSites()
{
    jump_sites.clear();
    for(uint k = 0; k < owned_sites.size(); k++)
    {
        std::vector<Point> j_s;
        Point key = owned_sites[k];
        for(int i = -2; i <=2; i++)
        {
            for( int j = -2; j <= 2; j++)
            {
                int x_ = key.x + i;
                int y_ = key.y + j;
                if(x_ >= 0 && x_ < X && y_ >= 0 && y_ < Y)
                {
                    if((*board)[x_][y_] == UNCLAIMED && (abs(key.x-x_ + key.y - y_) > 2 || abs(key.x - x_) >= 2 || abs(key.y - y_) == 2))
                    {
                        j_s.push_back(Point(x_,y_));
                    }
                }
            }
        }
        //qDebug() << id <<": Jumsp Sites for Owned site" << key.x << "," << key.y;
        //dispVector(j_s);
        jump_sites.insert({key,j_s});
    }
}

std::vector<Point> Player::getJumpSites(Point p)
{
    //qDebug() << id << ": get Jump sites for " << p.x << p.y;
    return jump_sites.at(p);
}

void Player::populateOwnedSites()
{
    /** Reads entire (*board) - populates owned_sites vector and sets score
      * TODO : On fly computation - there are enough constraints that we
      * know this without looking at (*board).
    **/
    owned_sites.resize(0);
    for(int i = 0; i < X; i++)
    {
        for(int  j = 0; j < Y; j++)
        {
            if((*board)[i][j] == id)
            {
                owned_sites.push_back(Point(i,j));
            }
        }
    }
    score = owned_sites.size();
    //qDebug() << QString::fromStdString(name) << " has " << QString(score) << "owned sites";
    setCloneableSites();
}


void Player::eraseOwnedSites()
{
    owned_sites.resize(0);
    //qDebug() << QString::fromStdString(name) << " owned sites erased :" << owned_sites.size();
}

void Player::setCloneableSites()
{
    bool c_sites[X][Y];
    for(int i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            c_sites[i][j] = false;
        }
    }
    cloneable_sites.resize(0);
    for(uint i = 0; i < owned_sites.size(); i++)
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
                    if((*board)[x_][y_] == UNCLAIMED && !c_sites[x_][y_])
                    {
                        cloneable_sites.push_back(Point(x_,y_));
                        c_sites[x_][y_] = true;
                    }

                }
            }
        }

    }
   //qDebug() << QString::fromStdString(name) <<"Displaying clonebale Size" << cloneable_sites.size();
    //dispVector(cloneable_sites);
}
std::vector<Point> Player::getCloneableSites()
{
    return cloneable_sites;
}

/** Debug helper functions **/

void Player::dispBoard()
{
    QDebug out(QtDebugMsg);

    out << QString::fromStdString(name) << " - (*board):\n";
    for(int i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            if((*board)[i][j] != -1)
                out << (int)(*board)[i][j];
            else
                out << "." ;
        }
        out << endl;
    }
}

void Player::dispVector(std::vector<Point> vec)
{
    qDebug() << QString::fromStdString(name) << ":";
    for(uint i = 0; i < vec.size(); i++)
    {
        qDebug() << vec[i].x << "," << vec[i].y;
    }
}

uint Player::getAvailableMoves()
{
    uint available_moves = 0;
    for ( auto it = jump_sites.begin(); it != jump_sites.end(); ++it)
    {
       available_moves+=it->second.size();
    }

    available_moves += cloneable_sites.size();
    return available_moves;
}
