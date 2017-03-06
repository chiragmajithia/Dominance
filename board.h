#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QHash>
#include <QDebug>
#include <QSignalMapper>

namespace Ui {
class Board;
}

class Board : public QMainWindow
{
    Q_OBJECT

public:
    explicit Board(int x, int y, QStringList p, QWidget *parent = 0);
    ~Board();

private slots:
    void on_pb_quit_clicked();
    void on_gb_clicked();

private:
    /** GUI Stuff **/
    Ui::Board *ui;
    QPair<int,int> key;
    QHash<QPair<int,int>, QPushButton*> button2d;

    /** Board Stuff **/
    int X, Y, N; //Board Dimensions and Players
    QStringList players; // Names of Players.

};

#endif // BOARD_H
