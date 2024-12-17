#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), gameMenu(nullptr), lobbyWindow(nullptr) {
    ui->setupUi(this);

    stackedWidget = new QStackedWidget(this);

    gameMenu = new GameMenu(this);
    lobbyWindow = new LobbyWindow(this);
    createLobbyWindow = new CreateLobbyWindow(this);

    stackedWidget->addWidget(gameMenu);
    stackedWidget->addWidget(lobbyWindow);
    stackedWidget->addWidget(createLobbyWindow);

    setCentralWidget(stackedWidget);

    setupMenu();
    setupConnections();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupMenu() {
    connect(gameMenu, &GameMenu::startGame, this, &MainWindow::onStartGame);
    connect(gameMenu, &GameMenu::exitGame, this, &MainWindow::close);
    connect(gameMenu, &GameMenu::fullScreenClicked, this, [this]() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });
}

void MainWindow::setupConnections() {
    connect(lobbyWindow, &LobbyWindow::openCreateLobby, this, &MainWindow::onOpenCreateLobby);
    connect(lobbyWindow, &LobbyWindow::backToMenu, this, &MainWindow::onBackToMenu);
    connect(createLobbyWindow, &CreateLobbyWindow::backToLobby, this, &MainWindow::onBackToLobby);
}

void MainWindow::onStartGame() {
    stackedWidget->setCurrentWidget(lobbyWindow);
}

void MainWindow::onOpenCreateLobby() {
    qDebug() << "Changing to CreateLobbyWindow.";
    stackedWidget->setCurrentWidget(createLobbyWindow);
}

void MainWindow::onBackToMenu() {
    stackedWidget->setCurrentWidget(gameMenu);
}

void MainWindow::onBackToLobby() {
    stackedWidget->setCurrentWidget(lobbyWindow);
}
