#include "createlobbywindow.h"
#include "Interface/filemanager.h"
#include "lobbywindow.h"
#include <QHeaderView>
#include <QFileInfoList>
#include <QMessageBox>

CreateLobbyWindow::CreateLobbyWindow(QWidget *parent)
    : QWidget(parent), connectionManager(new ConnectionManager(this))
{
    setupUI();
    loadSavedGames();
}

CreateLobbyWindow::~CreateLobbyWindow() {
    delete connectionManager;
}

void CreateLobbyWindow::setupUI() {
    this->setFixedSize(1280, 720);
    this->setWindowTitle("Create Lobby");

    QPixmap backgroundPixmap(":/resources/pozadina.png");
    backgroundPixmap = backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    savedGamesTable = new QTableWidget(this);
    savedGamesTable->setColumnCount(1);
    savedGamesTable->setHorizontalHeaderLabels({"Saved Games"});
    savedGamesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    savedGamesTable->setFixedWidth(300);
    savedGamesTable->setFixedHeight(450);
    savedGamesTable->setStyleSheet(R"(
        QTableWidget {
            background-color: rgba(255, 228, 196, 180);
            border: 2px solid rgba(255, 215, 0, 150);
            border-radius: 10px;
            font: 16px "Serif";
        }
        QHeaderView::section {
            background-color: rgba(139, 69, 19, 220);
            color: white;
            font: bold 18px "Serif";
        }
    )");

    QWidget *armyBox = new QWidget();
    armyBox->setStyleSheet(R"(
        background-color: rgba(255, 228, 196, 180);
        border: 2px solid rgba(255, 215, 0, 150);
        border-radius: 10px;
    )");
    armyBox->setFixedSize(300, 120);

    QVBoxLayout *armyBoxLayout = new QVBoxLayout(armyBox);

    QLabel *armyLabel = new QLabel("Choose Army:");
    armyLabel->setStyleSheet("font: bold 18px 'Serif'; color: brown;");

    armyComboBox = new QComboBox(this);
    armyComboBox->addItems({"Hajduk", "Janissary"});
    armyComboBox->setFixedWidth(250);
    armyComboBox->setStyleSheet(R"(
        QComboBox {
            background-color: rgba(139, 69, 19, 180);
            border: 1px solid rgba(255, 215, 0, 150);
            border-radius: 10px;
            padding: 5px;
            color: white;
        }
        QComboBox QAbstractItemView {
            border: 1px solid rgba(139, 69, 19, 180);
            border-radius: 5px;
            background-color: rgba(255, 215, 0, 150);
        }
    )");

    armyBoxLayout->addWidget(armyLabel);
    armyBoxLayout->addWidget(armyComboBox);
    armyBoxLayout->addStretch();

    leftLayout->addWidget(savedGamesTable);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(armyBox);
    leftLayout->addStretch();
    leftLayout->setContentsMargins(100, 70, 50, 50);

    QVBoxLayout *rightLayout = new QVBoxLayout();

    QWidget *rightBox = new QWidget();
    rightBox->setFixedSize(500, 580);
    rightBox->setStyleSheet(R"(
        background-color: rgba(255, 228, 196, 180);
        border: 2px solid rgba(255, 215, 0, 150);
        border-radius: 10px;
    )");

    QVBoxLayout *rightBoxLayout = new QVBoxLayout(rightBox);

    QLabel *player1Label = new QLabel("PLAYER 1");
    player1Label->setStyleSheet("font: bold 20px 'Serif'; color: brown;");
    player1Label->setAlignment(Qt::AlignCenter);

    player1ArmyLabel = new QLabel("Army: ");
    player1ArmyLabel->setStyleSheet("background-color: rgba(139, 69, 19, 180); font: 18px 'Serif'; color: white;");
    player1ArmyLabel->setAlignment(Qt::AlignLeft);

    QLabel *player2Label = new QLabel("PLAYER 2");
    player2Label->setStyleSheet("font: bold 20px 'Serif'; color: brown;");
    player2Label->setAlignment(Qt::AlignCenter);

    player2ArmyLabel = new QLabel("Army: ");
    player2ArmyLabel->setStyleSheet("background-color: rgba(139, 69, 19, 180); font: 18px 'Serif'; color: white;");
    player2ArmyLabel->setAlignment(Qt::AlignLeft);

    rightBoxLayout->addWidget(player1Label);
    rightBoxLayout->addWidget(player1ArmyLabel);
    rightBoxLayout->addSpacing(20);
    rightBoxLayout->addWidget(player2Label);
    rightBoxLayout->addWidget(player2ArmyLabel);
    rightBoxLayout->addSpacing(40);

    loadButton = new QPushButton("LOAD GAME");
    startButton = new QPushButton("START GAME");
    exitButton = new QPushButton("EXIT");

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
            color: #FFD700;
            font-size: 22px;
        }
        QPushButton:pressed {
            color: #FFA500;
        }
    )";

    loadButton->setStyleSheet(buttonStyle);
    startButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);
    loadButton->setFixedSize(400, 100);
    startButton->setFixedSize(400, 100);
    exitButton->setFixedSize(400, 100);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(exitButton);
    buttonLayout->setAlignment(Qt::AlignCenter);

    rightBoxLayout->addLayout(buttonLayout);

    rightBoxLayout->addStretch();

    rightLayout->addWidget(rightBox);

    rightLayout->setContentsMargins(100, 70, 50, 50);
    rightLayout->setAlignment(Qt::AlignTop);

    mainLayout->addLayout(leftLayout);
    mainLayout->addSpacing(70);
    mainLayout->addLayout(rightLayout);

    mainLayout->addStretch();

    setLayout(mainLayout);

    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
    connect(armyComboBox, &QComboBox::currentTextChanged, this, &CreateLobbyWindow::updateArmySelection);
    connect(startButton, &QPushButton::clicked, [this]() {
        if (!connectionManager->initializeServer()) {
            QMessageBox::warning(this, "Error", "Failed to start the server.");
            return;
        }

        if (!connectionManager->initializeClient()) {
            QMessageBox::warning(this, "Error", "Failed to connect the host client.");
            return;
        }

        if (connectionManager) {
            connect(connectionManager, &ConnectionManager::gameStarted, this, &CreateLobbyWindow::handleGameStart);
            qDebug() << "Signals connected successfully.";
        } else {
            qWarning() << "Failed to connect signals: ConnectionManager is nullptr.";
        }
    });
}

void CreateLobbyWindow::updateArmySelection(const QString &player1Army) {
    player1ArmyLabel->setText("Army: " + player1Army);

    QString player2Army = (player1Army == "Hajduk") ? "Janissary" : "Hajduk";
    player2ArmyLabel->setText("Army: " + player2Army);
}

void CreateLobbyWindow::loadSavedGames() {
    QDir directory("../Interface/saves/");
    QStringList jsonFiles = directory.entryList(QStringList() << "*.json", QDir::Files);

    savedGamesTable->setRowCount(0);

    FileManager fileManager;

    for (const QString &fileName : jsonFiles) {
        QString filePath = directory.filePath(fileName);

        if (fileManager.fileExists(filePath)) {
            int row = savedGamesTable->rowCount();
            savedGamesTable->insertRow(row);

            QTableWidgetItem *fileItem = new QTableWidgetItem(fileName);
            savedGamesTable->setItem(row, 0, fileItem);
        }
    }

    if (jsonFiles.isEmpty()) {
        QMessageBox::information(this, "No Saved Games", "No saved games were found in the directory.");
    }
}

void CreateLobbyWindow::handleGameStart() {
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
    close();
}
