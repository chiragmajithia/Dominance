#ifndef INIT_H
#define INIT_H

#include <QMainWindow>
#include <QDebug>
#include <QLineEdit>
#include <QVector>
#include "board.h"

namespace Ui {
class Init;
}

class Init : public QMainWindow
{
    Q_OBJECT

public:
    explicit Init(QWidget *parent = 0);
    ~Init();

private slots:

    void on_pb_commit_clicked();
    void on_pb_start_clicked();

private:

    /**
     * Game Parameters
     * TODO : Set the max and min values of the UI using this constants
    **/
    static const int N_MAX = 6, X_MAX = 16, Y_MAX = 16;

    int X_dim, Y_dim, N_players;

    /** UI Stuff  declarations **/
    Ui::Init *ui;
    QVector<QLineEdit*> led;
    Board *b;

    /** Function Declarations **/
    void populatePlayerList();
    void resetPlayerList();

};

#endif // INIT_H
