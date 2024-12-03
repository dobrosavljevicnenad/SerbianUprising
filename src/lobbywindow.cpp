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
    qDebug() << "Connected gameStarted signal to handleGameStarted slot.";

}

void LobbyWindow::onCreateServer() {


    serverManager = new ServerGameManager(&server, this);
    serverManager->startServer();
    QMessageBox::information(this, "Server Started", "Waiting for players to join...");

    // Host connects as Player 1

        clientManager = new ClientGameManager(nullptr);
        if (!clientManager->connectToServer()) {
            QMessageBox::warning(this, "Error", "Failed to connect the host client to the server.");
            return;
        }

}

void LobbyWindow::onJoinGame() {
        clientManager = new ClientGameManager(nullptr);
        if (!clientManager->connectToServer()) {
            QMessageBox::warning(this, "Error", "Failed to connect to the server.");
            return;
        }
        qDebug() << "Client 2 connected to the server.";

        connect(clientManager, &ClientGameManager::gameStarted, this, &LobbyWindow::handleGameStarted);
}

void LobbyWindow::handleGameStarted() {
    qDebug() << "handleGameStarted triggered";

    if (!clientManager) {
        qWarning() << "Error: clientManager is null.";
        return;
    }

    qDebug() << "Creating ClientWindow...";
    ClientWindow *gameWindow = new ClientWindow(clientManager, nullptr);
    qDebug() << "Something...";

    if (!gameWindow) {
        qWarning() << "Error: gameWindow creation failed.";
        return;
    }

    qDebug() << "Attempting to show ClientWindow...";
    gameWindow->show();
    qDebug() << "ClientWindow shown";

    close();
}
