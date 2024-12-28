#include "createlobbywindow.h"
#include "Interface/filemanager.h"
#include "lobbywindow.h"
#include <QHeaderView>
#include <QFileInfoList>
#include <qmessagebox.h>
#include "Interface/Items/custommessagebox.h"
#include <QStandardItemModel>


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
    //this->setFixedSize(1280, 720);
    this->setWindowTitle("Create Lobby");

    setBackgroundImage();

    outerLayout = new QVBoxLayout(this);
    mainLayout = new QHBoxLayout(this);

    leftLayout = new QVBoxLayout();
    savedGamesTable = new QTableWidget(this);
    savedGamesTable->setColumnCount(1);
    savedGamesTable->setHorizontalHeaderLabels({"Saved Games"});
    savedGamesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    savedGamesTable->setMinimumWidth(300);
    savedGamesTable->setMinimumHeight(450);
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

    connect(savedGamesTable, &QTableWidget::cellClicked, this, &CreateLobbyWindow::onFileClicked);

    armyBox = new QWidget();
    armyBox->setStyleSheet(R"(
        background-color: rgba(255, 228, 196, 180);
        border: 2px solid rgba(255, 215, 0, 150);
        border-radius: 10px;
    )");
    armyBox->setMinimumWidth(300);
    armyBox->setMinimumHeight(120);


    armyBoxLayout = new QVBoxLayout(armyBox);

    QLabel *armyLabel = new QLabel("Choose Army:");
    armyLabel->setStyleSheet("font: bold 18px 'Serif'; color: brown;");

    armyComboBox = new QComboBox(this);
    armyComboBox->addItem("Select Army");
    armyComboBox->addItems({"Hajduk", "Janissary"});
    armyComboBox->setMinimumWidth(250);
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

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(armyComboBox->model());
    QStandardItem* firstItem = model->item(0);
    firstItem->setEnabled(false);

    armyBoxLayout->addWidget(armyLabel);
    armyBoxLayout->addWidget(armyComboBox);
    armyBoxLayout->addStretch();

    leftLayout->addWidget(savedGamesTable);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(armyBox);
    leftLayout->addStretch();
    leftLayout->setContentsMargins(100, 70, 50, 50);

    rightLayout = new QVBoxLayout();

    rightBox = new QWidget();
    rightBox->setMinimumWidth(500);
    rightBox->setMinimumHeight(600);
    rightBox->setStyleSheet(R"(
        background-color: rgba(255, 228, 196, 180);
        border: 2px solid rgba(255, 215, 0, 150);
        border-radius: 10px;
    )");

    rightBoxLayout = new QVBoxLayout(rightBox);

    QLabel *player1Label = new QLabel("PLAYER 1");
    player1Label->setStyleSheet("font: bold 20px 'Serif'; color: brown;");
    player1Label->setAlignment(Qt::AlignCenter);

    player1ArmyLabel = new QLabel("Army: ");
    player1ArmyLabel->setStyleSheet("background-color: rgba(139, 69, 19, 180); font: 18px 'Serif'; color: white;");
    player1ArmyLabel->setAlignment(Qt::AlignLeft);

    ip = new QLabel("IP: ");
    ip->setStyleSheet("font: bold 20px 'Serif'; color: brown;");
    ip->setAlignment(Qt::AlignLeft);

    QLabel *player2Label = new QLabel("PLAYER 2");
    player2Label->setStyleSheet("font: bold 20px 'Serif'; color: brown;");
    player2Label->setAlignment(Qt::AlignCenter);

    player2ArmyLabel = new QLabel("Army: ");
    player2ArmyLabel->setStyleSheet("background-color: rgba(139, 69, 19, 180); font: 18px 'Serif'; color: white;");
    player2ArmyLabel->setAlignment(Qt::AlignLeft);

    rightBoxLayout->addWidget(player1Label);
    rightBoxLayout->addWidget(player1ArmyLabel);
    rightBoxLayout->addSpacing(10);
    rightBoxLayout->addWidget(ip);
    rightBoxLayout->addSpacing(20);
    rightBoxLayout->addWidget(player2Label);
    rightBoxLayout->addWidget(player2ArmyLabel);
    rightBoxLayout->addSpacing(30);

    loadButton = new QPushButton("LOAD GAME");
    startButton = new QPushButton("START GAME");
    exitButton = new QPushButton("EXIT");

    QString buttonStyle = R"(
        QPushButton {
            border: none;
            background: transparent;
            background-image: url(:/resources/Images/button.png);
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

    connect(loadButton, &QPushButton::clicked, this, &CreateLobbyWindow::onLoadGameClicked);

    buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(exitButton);
    buttonLayout->setAlignment(Qt::AlignCenter);

    rightBoxLayout->addLayout(buttonLayout);

    rightBoxLayout->addStretch();

    rightLayout->addWidget(rightBox);

    rightLayout->setContentsMargins(100, 70, 50, 50);
    rightLayout->setAlignment(Qt::AlignTop);

    mainLayout->addStretch();
    mainLayout->addLayout(leftLayout);
    mainLayout->addSpacing(70);
    mainLayout->addLayout(rightLayout);
    mainLayout->addStretch();

    outerLayout->addStretch();
    outerLayout->addLayout(mainLayout);
    outerLayout->addStretch();

    this->setLayout(outerLayout);

    armyComboBox->setCurrentText("Select Army");
    // updateArmySelection("Select Army");

    connect(exitButton, &QPushButton::clicked, this, &CreateLobbyWindow::backToLobby);
    connect(armyComboBox, &QComboBox::currentTextChanged, this, &CreateLobbyWindow::updateArmySelection);
    connect(startButton, &QPushButton::clicked, [this]() {
        if(isArmySelected){
            isGameStarted = true;
            int armyId = armyComboBox->currentIndex() + 1;
            QString localIp = connectionManager->getLocalIpAddress();
            CustomMessageBox::showMessage(QString("Server IP: %1").arg(localIp), this);
            if (!connectionManager->initializeServer()) {
                CustomMessageBox::showMessage("Error: Failed to start the server.", this);
                return;
            }

            if (!connectionManager->initializeClient()) {
                CustomMessageBox::showMessage("Error: Failed to connect the host client.", this);
                return;
            }

            ip->setText("IP: " + localIp);

            if (connectionManager) {
                connect(connectionManager, &ConnectionManager::gameStarted, this, &CreateLobbyWindow::handleGameStart);

                connectionManager->sendArmySelection(armyId);

                qDebug() << "Signals connected successfully.";
            } else {
                qWarning() << "Failed to connect signals: ConnectionManager is nullptr.";
            }
        } else {
            CustomMessageBox::showMessage("Please follow the correct sequence: first select a file, then load the game, and finally select the army.", this);
        }
    });

}

void CreateLobbyWindow::updateArmySelection(const QString &player1Army) {
    if (isGameLoaded && !isGameStarted) {
        player1ArmyLabel->setText("Army: " + player1Army);

        QString player2Army = (player1Army == "Hajduk") ? "Janissary" : "Hajduk";
        player2ArmyLabel->setText("Army: " + player2Army);

        ARMY1 = (player1Army == "Hajduk") ? ArmyType::HAJDUK : ArmyType::JANISSARY;
        ARMY2 = (player1Army == "Hajduk") ? ArmyType::JANISSARY : ArmyType::HAJDUK;

        isArmySelected = true;
        return;
    }

    if (isGameStarted && armyComboBox->currentIndex() != 0) {
        CustomMessageBox::showMessage("Game has already started. You cannot choose army anymore.", this);
        armyComboBox->setCurrentText("Select Army");
        return;
    }

    if (!isGameStarted && armyComboBox->currentIndex() != 0) {
        CustomMessageBox::showMessage("You have to Load Game first.", this);
        armyComboBox->setCurrentText("Select Army");
    }
}


void CreateLobbyWindow::loadSavedGames() {
    QDir directory("../../resources/saved_games/");
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
}

void CreateLobbyWindow::setBackgroundImage() {
    QPixmap backgroundPixmap(":/resources/Images/pozadina.png");
    backgroundPixmap = backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}


void CreateLobbyWindow::handleGameStart() {
    clientManager = connectionManager->getClientManager();
    if (selectedFile != nullptr)
        clientManager->loadGamePath = selectedFile;

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
}

void CreateLobbyWindow::onFileClicked(int row, int column) {
    QTableWidgetItem *item = savedGamesTable->item(row, column);

    if (item && !isGameLoaded) {
        selectedFile = item->text();
        CustomMessageBox::showMessage(QString("You clicked on \"%1\"").arg(selectedFile), this);
        isFileClicked = true;
        return;
    }

    if (item && isGameLoaded) {
        CustomMessageBox::showMessage(QString("Game is already loaded: \"%1\"").arg(selectedFile), this);
    }
}


void CreateLobbyWindow::onLoadGameClicked() {
    if (selectedFile.isEmpty()) {
        CustomMessageBox::showMessage("No file selected!", this);
        return;
    }

    if (isGameStarted) {
        CustomMessageBox::showMessage("Game has already started.", this);
        return;
    }

    if (isGameLoaded) {
        CustomMessageBox::showMessage("Game is already loaded.", this);
    } else {
        CustomMessageBox::showMessage(QString("Load Game: Loaded map: \"%1\"").arg(selectedFile), this);
        isGameLoaded = true;
    }
}


void CreateLobbyWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    int screenWidth = event->size().width();
    int screenHeight = event->size().height();

    const int baseWidth = 1280;
    const int baseHeight = 720;

    double scaleX = static_cast<double>(screenWidth) / baseWidth;
    double scaleY = static_cast<double>(screenHeight) / baseHeight;
    double scale = qMin(scaleX, scaleY);

    if(loadButton != nullptr){
        loadButton->setFixedSize(400 * scale, 100 * scale);
    }
    if(startButton != nullptr){
        startButton->setFixedSize(400 * scale, 100 * scale);
    }
    if(exitButton != nullptr){
        exitButton->setFixedSize(400 * scale, 100 * scale);
    }
    if(savedGamesTable != nullptr){
        savedGamesTable->setFixedSize(300 * scale, 450 * scale);
    }

    if(armyComboBox != nullptr){
        armyComboBox->setFixedWidth(250 * scale);
    }

    if(armyBox != nullptr){
        armyBox->setFixedSize(300 * scale, 120 * scale);
    }

    if(rightBox != nullptr){
        rightBox->setFixedSize(500 * scale, 600 * scale);
    }


    int labelFontSize = qMin(static_cast<int>(20 * scale), 30);
    player1ArmyLabel->setStyleSheet(QString("background-color: rgba(139, 69, 19, 180); font: %1px 'Serif'; color: white;").arg(labelFontSize));
    player2ArmyLabel->setStyleSheet(QString("background-color: rgba(139, 69, 19, 180); font: %1px 'Serif'; color: white;").arg(labelFontSize));
    ip->setStyleSheet(QString("font: bold %1px 'Serif'; color: brown;").arg(labelFontSize));

    int buttonFontSize = qMin(static_cast<int>(20 * scale), 30);
    QString buttonStyle;
    if (window()->windowState() & Qt::WindowMaximized) {
        buttonStyle = QString(R"(
            QPushButton {
                border: none;
                background: transparent;
                background-image: url(:/resources/Images/bigButton.png);
                background-position: center;
                background-repeat: no-repeat;
                color: white;
                font: bold %1px "Serif";
                text-align: center;
            }
            QPushButton:hover {
                background-image: url(:/resources/Images/bigButton.png);
                color: #FFD700;
                font-size: %2px;
            }
            QPushButton:pressed {
                background-image: url(:/resources/Images/bigButton.png);
                color: #FFA500;
            }
        )").arg(buttonFontSize).arg(buttonFontSize + 2);
    } else {
        buttonStyle = QString(R"(
            QPushButton {
                border: none;
                background: transparent;
                background-image: url(:/resources/Images/button.png);
                background-position: center;
                background-repeat: no-repeat;
                color: white;
                font: bold %1px "Serif";
                text-align: center;
            }
            QPushButton:hover {
                background-image: url(:/resources/Images/button.png);
                color: #FFD700;
                font-size: %2px;
            }
            QPushButton:pressed {
                background-image: url(:/resources/Images/button.png);
                color: #FFA500;
            }
        )").arg(buttonFontSize).arg(buttonFontSize + 2);
    }

    loadButton->setStyleSheet(buttonStyle);
    startButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    setBackgroundImage();
}
