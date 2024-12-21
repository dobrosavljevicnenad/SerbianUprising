#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "Interface/gamemenu.h"
#include "lobbywindow.h"
#include "createlobbywindow.h"

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
    CreateLobbyWindow *createLobbyWindow;
    QStackedWidget *stackedWidget;

    ServerGameManager* serverManager;
    ClientGameManager* clientManager;

    void setupMenu();

private slots:
    void showGameMenu();
    void showLobbyWindow();
    void showCreateLobbyWindow();

};

#endif // MAINWINDOW_H
