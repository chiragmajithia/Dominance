#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <QDebug>
#define UNCLAIMED -1
class Player
{
public:

    Player(){}

    Player(std::vector<std::vector<char> > &b, int id_, std::string n);

    struct Point{
        int x,y;
        Point(int x_ = 0, int y_ = 0):x(x_),y(y_){}
        QString out()
        {
            return QString::number(x) + "," + QString::number(y);
        }

        bool operator<(Point p1) const
        {
            if(x == p1.x)
            {
                if(y < p1.y)
                    return true;
                else
                    return false;
            }
            else if(x < p1.x)
                    return true;
            else
                    return false;
        }
    };

public:
    std::vector<std::vector<char> > board;
    std::string name;
    int id, score;
    int X, Y;

    std::vector<Point> owned_sites;

    void ownSites(const std::vector<Point> &indx);
    void populateOwnedSites();
    void dispVector(std::vector<Point> vec);
    std::vector<Point> getCloneableSites();
    void eraseOwnedSites();
};

#endif // PLAYER_H
