#include "init.h"
#include "ui_init.h"

Init::Init(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Init)
{
    ui->setupUi(this);
}

Init::~Init()
{
    delete ui;
}

void Init::on_pb_commit_clicked()
{
    /**
     * Fix the size of board and  Number of Players
     * Ask to Add Player Names
    **/
    if(ui->pb_commit->isChecked())
    {
        qDebug() << "Game Params Fixed ";
        ui->gb_game_params->setEnabled(false);
        X_dim = ui->sb_X_dim->value();
        Y_dim = ui->sb_Y_dim->value();
        N_players = ui->sb_N_players->value();
        populatePlayerList();
        ui->gb_players->setEnabled(true);
        ui->pb_commit->setText("Reset");
        ui->pb_start->setEnabled(true);
    }
    else
    {
        /** Delete Items from Scroll Area **/
        qDebug() << "Reset Game Params ";
        resetPlayerList();
        ui->gb_game_params->setEnabled(true);
        ui->gb_players->setEnabled(false);
        ui->pb_commit->setText("Commit");
        ui->pb_start->setEnabled(false);
    }
}

void Init::populatePlayerList()
{
    /** Populates Scroll Area with Editable Player Names **/
       QWidget *central = new QWidget;
       QVBoxLayout *layout = new QVBoxLayout(central);
       ui->sa_players->setWidget(central);
       ui->sa_players->setWidgetResizable(true);
       led.resize(N_players);
       for(int i = 0; i < N_players; i++)
       {
           led[i] = new QLineEdit(QString("Player %0").arg(i+1));
           layout->addWidget(led[i]);
       }
       layout->addStretch();
}

void Init::resetPlayerList()
{
    /** Free all widgets inside scroll area **/
    QWidget *central = ui->sa_players->widget();
        if(central)
            central->deleteLater();
   qDebug() << "List View Items removed";
}


void Init::on_pb_start_clicked()
{
    /**
     * Saves the Players Names in QStringList with mappings to Player id
     * Launches BoardUI with Player Names.
     **/
    setEnabled(false);
    QStringList players;
    for(int  i = 0; i < N_players; i++)
    {
        players.append(led[i]->text());
    }

    qDebug() <<"starting board with " <<  players;
    b = new Board(X_dim,Y_dim,players,this);
    b->show();
    hide();
}

void Init::on_pb_load_clicked()
{
    /** TODO:
     * Load Game Parameters and Players from File
     * Revisit Board.ui - add Save Game button near Quit.
    **/
}
