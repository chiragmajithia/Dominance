#include "board.h"
#include "ui_board.h"

Board::Board(int x, int y, QStringList &p_n, QWidget *parent) :X(x),Y(y),N(p_n.size()),QMainWindow(parent),ui(new Ui::Board), INIT_WITH(2)
{
    /**
     * Initialize Board paramters generate Layout. (Done)
     * Intialize Score Table. (Done)
     * Connect the buttons to a Slot() - get the address of the sender. (Done)
     * Create  and Initialize Players -> One vs One, One vs Comp, Comp vs Comp.. (Remove the slots?)
     * Initialize Board with 'Begin Game' Configuration. (Done)
     * Connect SLOTs with Signal Mapper - Did not work.
     * RESOLVE QMetaObject::connectSlotsByName: No matching signal for on_gb_clicked()
    **/
    bindx_clicked = QPair<int,int>(-1,-1); //invalid index;   // Currently selected button
    b_tmp_style = "";    //

    ui->setupUi(this);

    ui->pb_commit->setEnabled(false);


    for(int i = 0; i < N; i++)
    {
        ui->t_score->insertRow(i);
        ui->t_score->setItem(i,0,new QTableWidgetItem(p_n[i]));
    }

    this->setMaximumSize(this->size());
    loadButtons(); //load number of buttons - fix sizes and stuff.
    initBoard(); //Initialize copy of board;
    initPlayers(p_n); //Initialize Players with their names.
    notifyAllPlayers(); //notifyAllPlayers about their sites
    runAI();

}

Board::~Board()
{
    /** TODO
     * Confirm if the board pointers are deleted properly
     **/
    qDebug() <<"Board deleted";
    delete ui;
}

void Board::gb_clicked()
{


}




inline void Board::resetStyle(QPair<int,int> key, QString style)
{
    button2d.value(key)->setStyleSheet(style);
    button2d.value(key)->setChecked(false);
}

void Board::on_pb_commit_clicked()
{
    /**
     * Check the action taken by the player - clone or jump
     * Set Score for the player
     * Update Action Lists
     * Change owner of the board to next player (Done)
     * Update Score Lists with current player
     * Enable the relevant buttons on the grid for the next player
    **/
   // qDebug() <<" status click" << ui->pb_commit->text() << (ui->pb_commit->text().toStdString() == "Clone");
    if(ui->pb_commit->text().toStdString() == "Clone")
    {
        clone_commit_clicked();
    }
    else if(ui->pb_commit->text().toStdString() == "Jump")
    {
        jump_commit_clicked();
    }
    /** reset button selected **/
    bindx_clicked.first = -1;
    bindx_clicked.second = -1;

    notifyAllPlayers(); //notify all players of the change

    /** Update score on UI **/

    ui->pb_commit->setEnabled(false);

    /**
     * Check for game over -- for N players, skips intermediate players
     * with no moves -- a small modification, if player does not own any
     * sites, the player is out, can be made - its trivial
    **/
    int id = board_owner->id;
    for(int  i = 0; i < N; i++)
    {
        nextBoardOwner(); // give the ownership of board to next player

        if(board_owner->playerType() == 1)
        {
            runAI();
            qDebug() << "AI HERE!! RUN!!!" ;

        }
        else
        {

            qDebug() << "Huh! Muggle";
        }

        if(!board_owner->getAvailableMoves())
        {
            QMessageBox::information(this,"No available Moves",QString::fromStdString(board_owner->name) +
                                                          QString(" has no valid moves..Turn Skipped"));
            continue;
        }
        else
        {
            break;
        }
    }

    if(id == board_owner->id)
    {
        QMessageBox::StandardButton reply = QMessageBox::warning(this,"Game Over!",QString::fromStdString(players[getWinner()]->name) + QString::fromStdString(" wins !!!\n")
                                             + QString::fromStdString("Return Configuration Page?"),QMessageBox::Yes | QMessageBox::No);
        if(reply  == QMessageBox::Yes)
            on_pb_quit_clicked();
        qDebug() << "Game over";
    }
}

inline void Board::runAI()
{
    AI* ai_player = dynamic_cast<AI*>(board_owner);
    if(ai_player!= NULL)
    {
        Move best_move = ai_player -> getBestMove();
        if(best_move.action == Action::clone)
        {
            QPushButton *b = button2d.value(QPair<int,int>(best_move.to.x,best_move.to.y));
            b->setStyleSheet("border:5px solid rgb(255,255,128);"+clnble_color[board_owner->id]);
            b->setChecked(true);
            bindx_clicked = QPair<int,int>(best_move.to.x,best_move.to.y);
            ui->pb_commit->setText("Clone");
            ui->pb_commit->setEnabled(true);
        }
        else if(best_move.action == Action::jump)
        {
            QPushButton *b = button2d.value(QPair<int,int>(best_move.to.x,best_move.to.y));
            b->setStyleSheet("border:5px solid rgb(255,255,128);"+clnble_color[board_owner->id]);
            b->setChecked(true);
            b = button2d.value(QPair<int,int>(best_move.from.x,best_move.from.y));
            b->setStyleSheet("border:5px solid rgb(128,255,255);"+bckgnd_color[board_owner->id]);
            b->setChecked(true);
            bindx_jump_parent = QPair<int,int>(best_move.from.x,best_move.from.y);
            bindx_clicked = QPair<int,int>(best_move.to.x,best_move.to.y);
            ui->pb_commit->setText("Jump");
            ui->pb_commit->setEnabled(true);
        }
        qDebug() << "Cast Suceeded";
    }
}

inline uint Board::getWinner()
{
    uint max_score = 0, id = 0;
    for(int i = 0; i < N; i++)
    {
        if(max_score < players[i]->score)
        {
            max_score = players[i]->score;
            id = i;
        }
    }
    return id;
}

inline void Board::clone_commit_clicked()
{
    updateBoardAfterMove(); //Assign cite to owner
    /** Set GUI for the assigned button **/
    setButtonOwnColor(bindx_clicked.first,bindx_clicked.second,board_owner->id);
    setButtonEnable(bindx_clicked.first,bindx_clicked.second,false);
    setButtonChecked(bindx_clicked.first,bindx_clicked.second,false);
    button2d.value(bindx_clicked)->setEnabled(false);
    ui->t_actions->insertRow(0);
    ui->t_actions->setItem(0,0,new QTableWidgetItem(QString::fromStdString(board_owner->name)));
    std::string action = "C:" + std::to_string(bindx_clicked.first) + "," + std::to_string(bindx_clicked.second);
    ui->t_actions->setItem(0,1,new QTableWidgetItem(QString::fromStdString(action)));

}



inline void Board::jump_commit_clicked()
{
    //qDebug() << "jump from "  << bindx_jump_parent << " to "<< bindx_clicked;
    updateBoardAfterMove();
    board[bindx_jump_parent.first][bindx_jump_parent.second] = UNCLAIMED;
   // board[bindx_clicked.first][bindx_clicked.second] = board_owner->id; //Assign cite to owner
    /** Set GUI for the assigned button **/
    setButtonOwnColor(bindx_clicked.first,bindx_clicked.second,board_owner->id);
    setButtonNeutralColor(bindx_jump_parent.first,bindx_jump_parent.second);
    setButtonChecked(bindx_clicked.first,bindx_clicked.second,false);
    button2d.value(bindx_clicked)->setEnabled(false);
    button2d.value(bindx_jump_parent)->setChecked(false);
    button2d.value(bindx_jump_parent)->setEnabled(false);
    ui->t_actions->insertRow(0);
    ui->t_actions->setItem(0,0,new QTableWidgetItem(QString::fromStdString(board_owner->name)));
    std::string action = "J:" + std::to_string(bindx_jump_parent.first) + "," + std::to_string(bindx_jump_parent.second) + "->" + std::to_string(bindx_clicked.first) + "," + std::to_string(bindx_clicked.second);
    ui->t_actions->setItem(0,1,new QTableWidgetItem(QString::fromStdString(action)));
}


void Board::updateBoardAfterMove()
{
    //qDebug() << "jump from "  << bindx_jump_parent << " to "<< bindx_clicked;
    int x = bindx_clicked.first;
    int y = bindx_clicked.second;
    board[x][y] = board_owner->id;
    for(int  i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            int x_ = x + i;
            int y_ = y + j;
            if(x_ >= 0 && x_ < X && y_ >= 0 && y_ <Y)
            {
                if(board[x_][y_] != UNCLAIMED)
                {
                    setButtonOwnColor(x_,y_,board_owner->id);
                    board[x_][y_] = board_owner->id;
                }
            }
        }
    }
}

inline void Board::nextBoardOwner()
{
    int id = board_owner->id;
    id++;
    if(id == N)
        id = 0;
    board_owner = players[id];
    ui->t_score->selectRow(id);
    //qDebug() << "player changed! new owner " << QString::fromStdString(board_owner->name);
}



void Board::notifyAllPlayers()
{
    /** Parse the board and notify all players about their sites
      * i.e. Player->owned is updated
      * Erasing all owned_sites before this -> Better Way to do this?
      * Recompute players cloneable sites
      * Recompute players jump sites
      **/

    for(int i = 0; i < N; i++) // Delete all owned sites
        players[i]->eraseOwnedSites();

    for(int  i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            if(board[i][j] != -1)
            {
                //qDebug() << "["<<i<<","<<j<<"] = " << (int)board[i][j];
                Point pnt(i,j);
                std::vector<Point> index;
                index.push_back(pnt);
                players[board[i][j]]->ownSites(index);
            }
        }
    }

    for(int  i = 0; i < N; i++)
    {
        players[i]->setCloneableSites();
        players[i]->setJumpSites();
        players[i]->updateScore();
        ui->t_score->setItem(players[i]->id,1,new QTableWidgetItem(QString::number(players[i]->score)));
    }

}

void Board::on_pb_quit_clicked()
{
    /** Check if this is correct way to quit ***/
    hide();
    this->parentWidget()->show();
    this->parentWidget()->setEnabled(true);
    this->deleteLater();
}

inline void Board::initPlayers(QStringList &p_n)
{

    for(int i = 0; i < N-1; i++)
    {
        players.push_back( new AI(&board,i,p_n[i].toStdString()));
    }
    players.push_back( new AI(&board,N-1,p_n[N-1].toStdString()));
    board_owner = players[0];
    ui->t_score->selectRow(0);
    initSpawn();


}

void Board::initSpawn()
{
    if(N == 2)
    {
        board[0][0] = 0;
        board[X-1][Y-1] = 0;
        setButtonOwnColor(0,0,0);
        setButtonOwnColor(X-1,Y-1,0);
        board[X-1][0] = 1;
        board[0][Y-1] = 1;
        setButtonOwnColor(0,Y-1,1);
        setButtonOwnColor(X-1,0,1);
    }
    else
    {
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
                    setButtonOwnColor(x,y,i);
                    init_with ++;
                }
            }
        }
    }
}

void Board::loadButtons()
{
    for(int i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            QPair<int,int> p(i,j);
            QPushButton *b = new QPushButton;
            button2d.insert(p,b);
            b->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
            b->setEnabled(false);
            b->setCheckable(false);
            ui->gridLayout->addWidget(b,i,j);
            connect(b, SIGNAL(clicked()), this , SLOT(gb_clicked()));
        }
    }
}

inline void Board::setButtonOwnColor(int b_idx, int b_idy , int color_id)
{
    button2d.value(QPair<int,int>(b_idx,b_idy))->setStyleSheet(bckgnd_color[color_id]);
}

inline void Board::setButtonEnable(int b_idx, int b_idy,bool val)
{
    button2d.value(QPair<int,int>(b_idx,b_idy))->setEnabled(val);
}

inline void Board::setButtonChecked(int b_idx, int b_idy,bool val)
{
    button2d.value(QPair<int,int>(b_idx,b_idy))->setChecked(val);
}

inline void Board::setClonableStyle(int b_idx, int b_idy, int color_id)
{
    button2d.value(QPair<int,int>(b_idx,b_idy))->setStyleSheet(clnble_color[color_id]);
}

inline void Board::setButtonNeutralColor(int b_idx, int b_idy)
{
    /** Removes style from button
     * Potential Use - after jump;
     **/
    button2d.value(QPair<int,int>(b_idx,b_idy))->setStyleSheet("");
}

inline void Board::initBoard()
{
    board.resize(X);
    for(int i = 0; i < X; i++)
    {
        board[i].resize(Y, -1);
    }
}

inline bool Board::isOwner(QPair<int,int> b_slctd)
{
    //qDebug() << "isOwner()" << (board[b_slctd.first][b_slctd.second] == board_owner->id);
    return (board[b_slctd.first][b_slctd.second] == board_owner->id);
}

const QStringList Board::bckgnd_color =  QStringList() << "background-color: rgb(255,0,0)"
                                                         << "background-color: rgb(0,255,0)"
                                                         << "background-color: rgb(0,0,255)"
                                                         << "background-color: rgb(0,0,0)"; //Stores backgrouind color for player owned sites in array (Current MAX of 4)
const QStringList Board::clnble_color = QStringList() << "background-color: rgb(255,200,200)"
                                                         << "background-color: rgb(200,255,200)"
                                                         << "background-color: rgb(200,200,255)"
                                                         << "background-color: rgb(200,200,100)";
