#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <QDebug>
#include <unordered_map>

#define UNCLAIMED -1
class Player
{
public:

    Player(): board(NULL){}

    Player(std::vector<std::vector<char> > *b, int id_, std::string n);

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
        bool operator==(Point p1) const
        {
            if(x == p1.x && y == p1.y)
            {
                return true;
            }
            else
                return false;
        }
    };

    struct KeyHasher
    {
      std::size_t operator()(const Point& k) const
      {
        using std::size_t;
        using std::hash;

        return ((hash<int>()(k.x)
                 ^ (hash<int>()(k.y) << 1)) >> 1);
      }
    };

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

    /** Debug Helper Functions **/
    void dispBoard();
    void dispVector(std::vector<Point> vec);

};

#endif // PLAYER_H
