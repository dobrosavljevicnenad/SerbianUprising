#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientwindow.h"
#include <QPixmap>
#include <QPalette>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), gameMenu(new GameMenu(this)),
    lobbyWindow(new LobbyWindow(this)),
    createLobbyWindow(new CreateLobbyWindow(this)),
    stackedWidget(new QStackedWidget(this))  {

    ui->setupUi(this);

    stackedWidget->addWidget(gameMenu);
    stackedWidget->addWidget(lobbyWindow);
    stackedWidget->addWidget(createLobbyWindow);

    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(gameMenu);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    connect(gameMenu, &GameMenu::newGame, this, &MainWindow::showLobbyWindow);
    connect(gameMenu, &GameMenu::exitGame, this, &MainWindow::close);
    connect(lobbyWindow, &LobbyWindow::createLobby, this, &MainWindow::showCreateLobbyWindow);
    connect(lobbyWindow, &LobbyWindow::backToMenu, this, &MainWindow::showGameMenu);
    connect(createLobbyWindow, &CreateLobbyWindow::backToLobby, this, &MainWindow::showLobbyWindow);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showGameMenu() {
    stackedWidget->setCurrentWidget(gameMenu);
}

void MainWindow::showLobbyWindow() {
    stackedWidget->setCurrentWidget(lobbyWindow);
}

void MainWindow::showCreateLobbyWindow() {
    gameMenu->musicPlayer->stop();
    stackedWidget->setCurrentWidget(createLobbyWindow);
}
