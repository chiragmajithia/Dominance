#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QHash>
#include <QDebug>
#include <QSignalMapper>
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
    QHash<QPair<int,int>, QPushButton*> button2d;

    /** Board Stuff **/
    int X, Y, N; //Board Dimensions and Players
    QStringList player_names; // Names of Players.
    std::vector<std::vector<char> > board;
    std::vector<Player> players;
    Player  *board_owner;


    /** Functions **/
    inline void initPlayers(QStringList &p_n);
    inline void initBoard();
    inline void nextBoardOwner();
};

#endif // BOARD_H
