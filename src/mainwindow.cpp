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
    QPixmap pixmap(":/resources/Images/logo.png");
    this->setWindowIcon(QIcon(pixmap));
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
    connect(lobbyWindow, &LobbyWindow::stopBackgroundMusic, this, [this]() {
        if (gameMenu->musicPlayer) {
            gameMenu->musicPlayer->stop();
            delete gameMenu->musicPlayer;
            gameMenu->musicPlayer = nullptr;
        }
    });

    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
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
    if(gameMenu->musicPlayer != nullptr){
        gameMenu->musicPlayer->stop();
        delete gameMenu->musicPlayer;
        gameMenu->musicPlayer = nullptr;
    }
    stackedWidget->setCurrentWidget(createLobbyWindow);
}
