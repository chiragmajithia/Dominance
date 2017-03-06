#include "board.h"
#include "ui_board.h"

Board::Board(int x, int y, QStringList p, QWidget *parent) :X(x),Y(y),players(p),QMainWindow(parent),ui(new Ui::Board)
{
    N = players.size();
    ui->setupUi(this);
    qDebug() << "Created Board with " << X << "," << Y <<" dimensions and" << players <<"players";

    //QSignalMapper *signalMapper = new QSignalMapper(this);
    //connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(on_gb_clicked(int)));

    for(int i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            QPair<int,int> p(i,j);
            QPushButton *b = new QPushButton;
            button2d.insert(p,b);
            b->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            ui->gridLayout->addWidget(b,i,j);
            //signalMapper->setMapping(b, i);
            connect(b, SIGNAL(clicked()), this , SLOT(on_gb_clicked()));
        }
    }

}

Board::~Board()
{
    qDebug() <<"Board deleted";
    delete ui;
}

void Board::on_gb_clicked()
{
    QPushButton *b = (QPushButton*) sender();
    qDebug() << "clicked " << button2d.key(b);
}

void Board::on_pb_quit_clicked()
{
    /** Check if this is correct way to quit ***/
    hide();
    this->parentWidget()->show();
    this->parentWidget()->setEnabled(true);
    this->deleteLater();
}
