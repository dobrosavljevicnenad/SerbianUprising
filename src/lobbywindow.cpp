#include "lobbywindow.h"
#include "createlobbywindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include "Interface/Items/CustomMessageBox.h"
#include "Interface/Items/CustomInputDialog.h"

LobbyWindow::LobbyWindow(QWidget *parent)
    : QWidget(parent), connectionManager(new ConnectionManager(this)) {

    setupUI();
    connectSignals();
}

LobbyWindow::~LobbyWindow() {
    delete connectionManager;
}

void LobbyWindow::setupUI(){
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    setBackgroundImage();

    buttonFrame = new QFrame(this);
    buttonFrame->setFixedSize(450, 350);
    buttonFrame->setStyleSheet(R"(
        background-color: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 rgba(255, 228, 196, 200),
            stop:1 rgba(139, 69, 19, 220)
        );
        border-radius: 10px;
        border: 2px solid rgba(255, 215, 0, 150);
    )");

    backButton = new QPushButton("BACK");
    createLobbyButton = new QPushButton("CREATE LOBBY");
    joinLobbyButton = new QPushButton("JOIN LOBBY");

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
    buttonLayout->addWidget(joinLobbyButton, 0, Qt::AlignCenter);
    buttonLayout->addWidget(backButton, 0, Qt::AlignCenter);
    buttonLayout->addStretch();

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonFrame, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

void LobbyWindow::setBackgroundImage() {
    QPixmap backgroundPixmap(":/resources/pozadina.png");
    backgroundPixmap = backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}


void LobbyWindow::connectSignals() {
    connect(backButton, &QPushButton::clicked, this, &LobbyWindow::returnToMenu);
    connect(createLobbyButton, &QPushButton::clicked, this, &LobbyWindow::createLobby);
    connect(joinLobbyButton, &QPushButton::clicked, this, &LobbyWindow::onJoinGame);
    if (connectionManager) {
        connect(connectionManager, &ConnectionManager::gameStarted, this, [this](){
            clientManager = connectionManager->getClientManager();
            qDebug() << "Game is starting."<< clientManager->ClientId;

            if (clientManager->ClientId == 1) {
                qDebug() << "Starting game for Player 1";
            } else if (clientManager->ClientId == 2) {
                qDebug() << "Starting game for Player 2";
            } else {
                qWarning() << "Unknown Client ID:" << clientManager->ClientId;
                return;
            }

            gameWindow = new ClientWindow(clientManager, nullptr);
            gameWindow->show();

            this->close();

            QWidget *parent = this->parentWidget();
            while (parent != nullptr) {
                QWidget *nextParent = parent->parentWidget();
                parent->close();
                parent = nextParent;
            }

        });
        qDebug() << "Signals connected successfully.";
    } else {
        qWarning() << "Failed to connect signals: ConnectionManager is nullptr.";
    }
}

void LobbyWindow::onCreateServer() {
    emit openCreateLobby();
}

void LobbyWindow::onJoinGame() {
    CustomInputDialog dialog("Join Game", "Enter host IP address:", this);
    if (dialog.exec() == QDialog::Accepted) {
        QString ipAddress = dialog.getInputText();
        if (ipAddress.isEmpty()) {
            qWarning() << "IP address entry canceled or empty.";
            return;
        }

        if (ipAddress == connectionManager->getLocalIpAddress()) {
            qDebug() << "Successfully connected to host at IP:" << ipAddress;
        } else {
            CustomMessageBox("Connection failed: Could not connect to the host. Please check the IP address and try again.", this);
            return;
        }

        if (!connectionManager->initializeClient()) {
            CustomMessageBox("Error: Failed to connect to the server.", this);
            return;
        }

        emit stopBackgroundMusic();

    } else {
        qWarning() << "Dialog canceled by user.";
    }
}

void LobbyWindow::returnToMenu()
{
    emit backToMenu();
    this->close();
}

QPushButton* LobbyWindow::getCreateLobbyButton() const {
    return createLobbyButton;
}

void LobbyWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    int screenWidth = event->size().width();
    int screenHeight = event->size().height();

    const int baseWidth = 960;
    const int baseHeight = 540;

    double scaleX = static_cast<double>(screenWidth) / baseWidth;
    double scaleY = static_cast<double>(screenHeight) / baseHeight;
    double scale = qMin(scaleX, scaleY);

    backButton->setFixedSize(400 * scale, 100 * scale);
    createLobbyButton->setFixedSize(400 * scale, 100 * scale);
    joinLobbyButton->setFixedSize(400 * scale, 100 * scale);

    buttonFrame->setFixedSize(450 * scale, 350 * scale);

    int fontSize = qMin(static_cast<int>(20 * scale), 30);
    QString buttonStyle = QString(R"(
        QPushButton {
            border: none;
            background: transparent;
            background-image: url(:/resources/button.png);
            background-position: center;
            background-repeat: no-repeat;
            color: white;
            font: bold %1px "Serif";
            text-align: center;
        }
        QPushButton:hover {
            background-image: url(:/resources/button.png);
            color: #FFD700;
            font-size: %2px;
        }
        QPushButton:pressed {
            background-image: url(:/resources/button.png);
            color: #FFA500;
        }
    )").arg(fontSize).arg(static_cast<int>(fontSize * 1.1));

    backButton->setStyleSheet(buttonStyle);
    createLobbyButton->setStyleSheet(buttonStyle);
    joinLobbyButton->setStyleSheet(buttonStyle);

    setBackgroundImage();
}
