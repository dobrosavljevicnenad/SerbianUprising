#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), gameMenu(nullptr), lobbyWindow(nullptr) {
    ui->setupUi(this);

    setupMenu();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupMenu() {
    gameMenu = new GameMenu(this);
    setCentralWidget(gameMenu);

    // Connect GameMenu signals
    connect(gameMenu, &GameMenu::startGame, this, [this]() {
        lobbyWindow = new LobbyWindow();
        lobbyWindow->show();
        this->close(); // Close MainWindow when entering lobby
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
