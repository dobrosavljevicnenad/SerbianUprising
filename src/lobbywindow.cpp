#include "lobbywindow.h"
#include "createlobbywindow.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QFormLayout>


LobbyWindow::LobbyWindow(QWidget *parent)
    : QWidget(parent), connectionManager(new ConnectionManager(this)) {

    setupUI();
    connectSignals();
}

LobbyWindow::~LobbyWindow() {
    delete connectionManager;
}

void LobbyWindow::setupUI(){
    //this->setFixedSize(1280, 720);
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setContentsMargins(10, 70, 10, 10);

    // QPixmap backgroundPixmap(":/resources/pozadina.png");
    // backgroundPixmap = backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // QPalette palette;
    // palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
    // this->setPalette(palette);
    // this->setAutoFillBackground(true);


    buttonFrame = new QFrame(this);
    buttonFrame->setMinimumWidth(450);
    buttonFrame->setMinimumHeight(350);
    buttonFrame->setStyleSheet(R"(
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 rgba(255, 228, 196, 200),
            stop:1 rgba(139, 69, 19, 220)
        );
        border-radius: 10px;
        border: 2px solid rgba(255, 215, 0, 150);
    )");

    backButton = new QPushButton("Main Menu");
    createLobbyButton = new QPushButton("Create Lobby");
    joinLobbyButton = new QPushButton("Join Lobby");

    QString buttonStyle = R"(
        QPushButton {
            border: none;
            background: transparent;
            background-image: url(:/resources/button.png);
            background-position: center;
            background-repeat: no-repeat;
            color: white;
            font: bold 20px "Serif";
            text-align: center;
        }
        QPushButton:hover {
            background-image: url(:/resources/button.png);
            color: #FFD700;
            font-size: 22px;
        }
        QPushButton:pressed {
            background-image: url(:/resources/button.png);
            color: #FFA500;
        }
    )";

    backButton->setStyleSheet(buttonStyle);
    createLobbyButton->setStyleSheet(buttonStyle);
    joinLobbyButton->setStyleSheet(buttonStyle);

    backButton->setFixedSize(400, 100);
    createLobbyButton->setFixedSize(400, 100);
    joinLobbyButton->setFixedSize(400, 100);

    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonFrame);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();
    buttonLayout->addWidget(createLobbyButton, 0, Qt::AlignCenter);
    //buttonLayout->addSpacing(10);
    buttonLayout->addWidget(joinLobbyButton, 0, Qt::AlignCenter);
    //buttonLayout->addSpacing(10);
    buttonLayout->addWidget(backButton, 0, Qt::AlignCenter);
    buttonLayout->addStretch();


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonFrame, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

void LobbyWindow::connectSignals() {
    connect(backButton, &QPushButton::clicked, this, &LobbyWindow::returnToMenu);
    connect(createLobbyButton, &QPushButton::clicked, this, &LobbyWindow::onCreateServer);
    connect(joinLobbyButton, &QPushButton::clicked, this, &LobbyWindow::onJoinGame);
}

void LobbyWindow::onCreateServer() {
    emit openCreateLobby();
}

void LobbyWindow::onJoinGame() {
    if (!connectionManager->initializeClient()) {
        QMessageBox::warning(this, "Error", "Failed to connect to the server.");
        return;
    }
}


void LobbyWindow::returnToMenu()
{
    emit backToMenu();
    this->close();
}
