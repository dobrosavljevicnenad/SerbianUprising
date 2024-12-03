#include "lobbywindow.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>


LobbyWindow::LobbyWindow(QWidget *parent)
    : QWidget(parent) {
    setupUI();
    connectSignals();
}

LobbyWindow::~LobbyWindow() {}

void LobbyWindow::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    createServerButton = new QPushButton("Create Server");
    joinGameButton = new QPushButton("Join Game");
    backButton = new QPushButton("Back");

    layout->addWidget(createServerButton);
    layout->addWidget(joinGameButton);
    layout->addWidget(backButton);

    setLayout(layout);
}

void LobbyWindow::connectSignals() {
    connect(createServerButton, &QPushButton::clicked, this, &LobbyWindow::onCreateServer);
    connect(joinGameButton, &QPushButton::clicked, this, &LobbyWindow::onJoinGame);
    connect(&server, &Server::gameStarted, this, &LobbyWindow::handleGameStarted);

}

void LobbyWindow::onCreateServer() {
    serverManager = new ServerGameManager();
    serverManager->startServer();

    QMessageBox::information(this, "Server Started", "Waiting for another player to join...");

    clientManager = new ClientGameManager(nullptr);
    if (!clientManager->connectToServer()) {
        QMessageBox::warning(this, "Error", "Failed to connect to the server.");
        return;
    }

}

void LobbyWindow::onJoinGame() {
    clientManager = new ClientGameManager(nullptr);
    if (!clientManager->connectToServer()) {
        QMessageBox::warning(this, "Error", "Failed to connect to the server.");
        return;
    }
}

void LobbyWindow::handleGameStarted() {
    ClientWindow *gameWindow = new ClientWindow(clientManager, this);
    gameWindow->show();
    close();
}
