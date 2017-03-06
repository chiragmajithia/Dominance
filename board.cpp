#include "board.h"
#include "ui_board.h"

Board::Board(int x, int y, QStringList &p_n, QWidget *parent) :X(x),Y(y),QMainWindow(parent),ui(new Ui::Board)
{
    /** TODO:
     * Initialize Board paramters generate Layout. (Done)
     * Intialize Score Table. (Done)
     * Connect the buttons to a Slot() - get the address of the sender. (Done)
     * Create  and Initialize Players -> One vs One, One vs Comp, Comp vs Comp.. (Remove the slots?)
     * Initialize Board with 'Begin Game' Configuration.
     * Connect SLOTs with Signal Mapper - Did not work.
     * RESOLVE QMetaObject::connectSlotsByName: No matching signal for on_gb_clicked()
    **/

    ui->setupUi(this);

    N = p_n.size();
    ui->pb_commit->setEnabled(false);

    for(int i = 0; i < N; i++)
    {
        ui->t_score->insertRow(i);
        ui->t_score->setItem(i,0,new QTableWidgetItem(p_n[i]));
    }

    for(int i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            QPair<int,int> p(i,j);
            QPushButton *b = new QPushButton;
            button2d.insert(p,b);
            b->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            b->setCheckable(true);
            ui->gridLayout->addWidget(b,i,j);
            connect(b, SIGNAL(clicked()), this , SLOT(on_gb_clicked()));
        }
    }
    initPlayers(p_n); //Initialize Players with their names.
    initBoard(); //Initialize copy of board;
    qDebug() << "Created Board with " << X << "," << Y <<" dimensions and" << player_names <<"players";
}

Board::~Board()
{
    /** TODO
     * Confirm if the board pointers are deleted properly
     **/
    qDebug() <<"Board deleted";
    delete ui;
}

void Board::on_gb_clicked()
{
    /** TODO
     * Simulate actions of current player - update Score Table..
     * Clone - show valid clone sites (Current)
     * if(click is on clonable site)
     * clone actions are valid -- Enable Commit!
     * if(click is on owned site)
     * set siteSelected == true - disable clonable sites
     * enable and view potential jump sites.
     * if(siteSelected == true)
     **/
    QPushButton *b = (QPushButton*) sender();
    qDebug() << "clicked " << button2d.key(b) << " by " << QString::fromStdString(board_owner->name);
    ui->pb_commit->setEnabled(true);
    b->setStyleSheet("QPushButton:checked {background-color: rgb(225, 225, 0);border-style: inset;}");
}



void Board::on_pb_commit_clicked()
{
    /**
     * TODO:
     * Check the action taken by the player - clone or jump
     * Set Score for the player
     * Update Action Lists
     * Change owner of the board to next player (Done)
     * Update Score Lists with current player
     * Enable the relevant buttons on the grid for the next player
    **/
    nextBoardOwner();

}

inline void Board::initBoard()
{
    board.resize(Y);
    for(int i = 0; i < Y; i++)
    {
        board[i].resize(X, -1);
    }
}

inline void Board::initPlayers(QStringList &p_n)
{
    players.resize(N);
    for(int i = 0; i < N; i++)
    {
        Player p(board,i,p_n[i].toStdString());
        players[i] = p;
    }
    board_owner = &players[0];
    ui->t_score->selectRow(0);
}

inline void Board::nextBoardOwner()
{
    int id = board_owner->id;
    id++;
    if(id == N)
        id = 0;
    board_owner = &players[id];
    ui->t_score->selectRow(id);
    qDebug() << "player changed! new owner " << QString::fromStdString(board_owner->name);
}

void Board::on_pb_quit_clicked()
{
    /** Check if this is correct way to quit ***/
    hide();
    this->parentWidget()->show();
    this->parentWidget()->setEnabled(true);
    this->deleteLater();
}

