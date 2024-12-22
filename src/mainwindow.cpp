#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), gameMenu(nullptr), lobbyWindow(nullptr) {
    ui->setupUi(this);
    setupMenu();

    this->showMaximized();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupMenu() {
    gameMenu = new GameMenu(this);
    setCentralWidget(gameMenu);
    this->showMaximized();
    // Connect GameMenu signals
    connect(gameMenu, &GameMenu::startGame, this, [this]() {
        lobbyWindow = new LobbyWindow();
        lobbyWindow->showMaximized();
        lobbyWindow->show();
        this->close();
    });

    connect(gameMenu, &GameMenu::exitGame, this, &MainWindow::close);

    connect(gameMenu, &GameMenu::fullScreenClicked, this, [this]() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });
}


