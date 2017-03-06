#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QHash>
#include <QDebug>
#include <QSignalMapper>
#include <cstdlib>
#include "include/player.h"


namespace Ui {
class Board;
}

class Board : public QMainWindow
{
    Q_OBJECT

public:    
    explicit Board(int x, int y, QStringList &p, QWidget *parent = 0);
    ~Board();

private slots:
    void on_pb_quit_clicked();
    void on_gb_clicked();
    void on_pb_commit_clicked();

private:
    /** GUI Stuff **/
    Ui::Board *ui;
    QPair<int,int> key;
    QHash<QPair<int,int>, QPushButton*> button2d; //Use Player::Point structure?

    /** Board Stuff **/
    const int X, Y, N, INIT_WITH = 2; //Board Dimensions and Players. Initializes with 2 random positions
    const QStringList bckgnd_color = QStringList() << "background-color: rgb(255,0,0)"
                                                   << "background-color: rgb(0,255,0)"
                                                   << "background-color: rgb(0,0,255)"
                                                   << "background-color: rgb(0,0,0)";
    const QStringList clnble_color = QStringList() << "background-color: rgb(255,100,100)"
                                                   << "background-color: rgb(100,255,100)"
                                                   << "background-color: rgb(100,100,255)"
                                                   << "background-color: rgb(100,100,100)";

    QStringList player_names; // Names of Players.
    std::vector<std::vector<char> > board;
    std::vector<Player> players;
    Player  *board_owner; //use unique pointer?
    QPair<int,int> b_slctd;
    QString b_prop;

    /** Functions **/
    inline void initPlayers(QStringList &p_n);
    inline void initBoard();
    inline void nextBoardOwner();
    inline void setButtonColor(int b_idx, int b_idy, int color_id);
    inline void setClonableStyle(int b_idx, int b_idy, int color_id);
    inline void removeStyle(int b_idx, int b_idy);
    inline void resetStyle(QPair<int,int> key, QString style);

    void notifyAllPlayers();
    void enableOwnerSites();
    void disableOwnerSites();
};

#endif // BOARD_H
