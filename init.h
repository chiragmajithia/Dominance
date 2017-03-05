#ifndef INIT_H
#define INIT_H

#include <QMainWindow>

namespace Ui {
class Init;
}

class Init : public QMainWindow
{
    Q_OBJECT

public:
    explicit Init(QWidget *parent = 0);
    ~Init();

private:
    Ui::Init *ui;
};

#endif // INIT_H
