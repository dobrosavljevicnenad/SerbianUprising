#include "mainwindow.h"

#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/Images/logo.png"));
    a.setApplicationName("Ustanak");
    MainWindow w;
    w.setWindowTitle("Ustanak");
    w.show();
    return a.exec();
}
