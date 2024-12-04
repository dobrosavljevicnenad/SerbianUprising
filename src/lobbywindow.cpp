#include "lobbywindow.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>


LobbyWindow::LobbyWindow(QWidget *parent)
    : QWidget(parent), connectionManager(new ConnectionManager(this)) {
    setupUI();
    connectSignals();
}

LobbyWindow::~LobbyWindow() {
    delete connectionManager;
}

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
    if (connectionManager) {
        connect(connectionManager, &ConnectionManager::gameStarted, this, &LobbyWindow::handleGameStart);
        qDebug() << "Signals connected successfully.";
    } else {
        qWarning() << "Failed to connect signals: ConnectionManager is nullptr.";
    }
}

void LobbyWindow::onCreateServer() {
    if (!connectionManager->initializeServer()) {
        QMessageBox::warning(this, "Error", "Failed to start the server.");
        return;
    }

    if (!connectionManager->initializeClient()) {
        QMessageBox::warning(this, "Error", "Failed to connect the host client.");
        return;
    }

    QMessageBox::information(this, "Server Started", "Waiting for players to join...");
}

void LobbyWindow::onJoinGame() {
    if (!connectionManager->initializeClient()) {
        QMessageBox::warning(this, "Error", "Failed to connect to the server.");
        return;
    }

    //QMessageBox::information(this, "Client Connected", "Waiting for the game to start...");
}

void LobbyWindow::handleGameReady() {
    qDebug() << "Both clients are ready. Waiting to start the game.";
}

void LobbyWindow::handleGameStart() {
    clientManager = connectionManager->getClientManager();
    qDebug() << "Game is starting."<< clientManager->getId();;
    gameWindow = new ClientWindow(clientManager, nullptr);
    gameWindow->show();
    close();
}
