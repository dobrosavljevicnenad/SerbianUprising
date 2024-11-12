#include "mainwindow.h"

#include <QApplication>
#include "base/testBattle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    test_Battle();
    w.show();
    return a.exec();
}
