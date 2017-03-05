#include "init.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Init w;
    w.show();

    return a.exec();
}
