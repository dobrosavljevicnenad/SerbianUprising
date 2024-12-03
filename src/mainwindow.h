#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Interface/gamemenu.h"
#include "lobbywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameMenu *gameMenu;
    LobbyWindow *lobbyWindow;

    ServerGameManager* serverManager;
    ClientGameManager* clientManager;

    void setupMenu();
};

#endif // MAINWINDOW_H
