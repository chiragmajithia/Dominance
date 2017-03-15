#ifndef POINT
#define POINT

#include <QString>
namespace type{
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
    bool operator==(const Point p1) const
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

enum Action
{
    clone,
    jump,
    invalid = -1
};

struct Move
{
   Action action;
   Point from;
   Point to;
   float score;
   Move(Action a_,Point from_, Point to_, int score_ = -1):action(a_),from(from_),to(to_),score(score_) {}
   bool operator<(const Move &a) const
   {
       return score > a.score;
   }
};
}
#endif // POINT

