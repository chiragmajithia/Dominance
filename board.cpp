#include "board.h"
#include "ui_board.h"

Board::Board(int x, int y, QStringList &p_n, QWidget *parent) :X(x),Y(y),N(p_n.size()),QMainWindow(parent),ui(new Ui::Board)
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
            b->setEnabled(false);
            b->setCheckable(false);
            ui->gridLayout->addWidget(b,i,j);
            connect(b, SIGNAL(clicked()), this , SLOT(on_gb_clicked()));
        }
    }

    this->setMaximumSize(this->size());

    initBoard(); //Initialize copy of board;
    initPlayers(p_n); //Initialize Players with their names.
    notifyAllPlayers();
    enableOwnerSites();
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
    b->setStyleSheet("QPushButton:checked {background-color: rgb(225, 225, 0);"
                     "border-style: inset;}"
                     "QPushButton::!checked {"+bckgnd_color[board_owner->id]+"}");
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
    disableOwnerSites();
    notifyAllPlayers();
    nextBoardOwner();
    enableOwnerSites();
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

    for(int i = 0; i < N; i++)
    {
        int init_with = 0;
        while(init_with < INIT_WITH)
        {
            int indx = (rand()%(X*Y));
            int x = indx % (Y-1);
            int y = indx /(Y-1);
            if(board[x][y] == -1)
            {
                qDebug() << x << "," << y<<" for Player " << i;
                board[x][y] = i;
                setButtonColor(x,y,i);
                init_with ++;
            }
        }
    }
}

inline void Board::setButtonColor(int b_idx, int b_idy , int color_id)
{
    button2d.value(QPair<int,int>(b_idx,b_idy))->setStyleSheet(bckgnd_color[color_id]);
}

inline void Board::setClonableStyle(int b_idx, int b_idy, int color_id)
{
    button2d.value(QPair<int,int>(b_idx,b_idy))->setStyleSheet(clnble_color[color_id]);
}

inline void Board::removeStyle(int b_idx, int b_idy)
{
    /** Removes style from button
     * Potential Use - after jump;
     **/
    button2d.value(QPair<int,int>(b_idx,b_idy))->setStyleSheet("");
}

void Board::notifyAllPlayers()
{
    /** Parse the board and notify all players about their sites
      * i.e. Player->owned is updated
      * Consider erasing all owned_sites before this
      **/

    for(int i = 0; i < N; i++)
        players[i].eraseOwnedSites();

    for(int  i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            if(board[i][j] != -1)
            {
                qDebug() << "["<<i<<","<<j<<"] = " << (int)board[i][j];
                Player::Point pnt(i,j);
                std::vector<Player::Point> index;
                index.push_back(pnt);
                players[board[i][j]].ownSites(index);
            }
        }
    }
}

void Board::enableOwnerSites()
{
    /** Enables current player's currently owned sites (for jumps)
     * Enables current player's clonable sites
    **/
    Player::Point indx;
    QPushButton *b;
    for(int i = 0; i < board_owner->owned_sites.size(); i++)
    {
       indx = board_owner->owned_sites[i];
       b = button2d.value(QPair<int,int>(indx.x,indx.y));
       setButtonColor(indx.x,indx.y,board_owner->id);
       b->setEnabled(true);
       b->setCheckable(true);
    }

    std::vector<Player::Point> clonable_sites = board_owner->getCloneableSites(); // Consider making this class variable: Ease to reset
    qDebug() << QString::fromStdString(board_owner->name) << "got clonable sites: "<< QString::number(clonable_sites.size());
    for(int i = 0; i < clonable_sites.size(); i++)
    {
       indx = clonable_sites[i];
       //qDebug() << "Indx :" << indx.x << "," << indx.y;
       b = button2d.value(QPair<int,int>(indx.x,indx.y));
       setClonableStyle(indx.x,indx.y,board_owner->id);
       b->setEnabled(true);
       b->setCheckable(true);
    }
}

void Board::disableOwnerSites()
{
    /** Disables current player's currently owned sites
     * Disables current player's clonable sites.
     * Called before shifting board ownership;
     **/
    Player::Point indx;
    QPushButton *b;
    for(int i = 0; i < board_owner->owned_sites.size(); i++)
    {
       indx = board_owner->owned_sites[i];
       b = button2d.value(QPair<int,int>(indx.x,indx.y));
       b->setEnabled(false);
       b->setCheckable(false);
    }
    std::vector<Player::Point> clonable_sites = board_owner->getCloneableSites(); // Consider making this class variable: Ease to reset
    qDebug() << QString::fromStdString(board_owner->name) << "got clonable sites: "<< QString::number(clonable_sites.size());
    for(int i = 0; i < clonable_sites.size(); i++)
    {
       indx = clonable_sites[i];
       b = button2d.value(QPair<int,int>(indx.x,indx.y));
       removeStyle(indx.x,indx.y);
       b->setEnabled(true);
       b->setCheckable(true);
    }
}
