#include "lobbywindow.h"
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
    this->setFixedSize(1280, 720);
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setContentsMargins(10, 70, 10, 10);

    QPixmap backgroundPixmap(":/resources/pozadina.png");
    backgroundPixmap = backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Game Name", "Players", "Availability"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->resizeRowsToContents();
    table->setStyleSheet(R"(
        QTableWidget {
            background-color: rgba(255, 228, 196, 180);
            border: 2px solid rgba(255, 215, 0, 150);
            border-radius: 10px;
        }
        QTableWidget::item {
            padding: 5px;
        }
        QHeaderView::section {
            background-color: rgba(139, 69, 19, 220);
            color: white;
            border: none;
            padding: 5px;
        }
    )");

    layout->addWidget(table);

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

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(createLobbyButton);
    buttonLayout->addWidget(joinLobbyButton);

    layout->addLayout(buttonLayout);

    // connect(backButton, &QPushButton::clicked, this, &LobbyWindow::returnToMenu);
    // connect(createLobbyButton, &QPushButton::clicked, this, &LobbyWindow::onCreateServer);
    // connect(joinLobbyButton, &QPushButton::clicked, this, &LobbyWindow::onJoinGame);
    setLayout(layout);
}

void LobbyWindow::connectSignals() {
    connect(createLobbyButton, &QPushButton::clicked, this, &LobbyWindow::onCreateServer);
    connect(joinLobbyButton, &QPushButton::clicked, this, &LobbyWindow::onJoinGame);
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

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Create Lobby");

    QFormLayout *formLayout = new QFormLayout(dialog);

    QLineEdit *gameNameEdit = new QLineEdit(dialog);
    QLineEdit *playerNameEdit = new QLineEdit(dialog);

    formLayout->addRow("Game Name", gameNameEdit);
    formLayout->addRow("Player", playerNameEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    formLayout->addWidget(buttonBox);

    if (dialog->exec() == QDialog::Accepted) {
        QString gameName = gameNameEdit->text();
        QString playerName = playerNameEdit->text();

        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(gameName));
        table->setItem(row, 1, new QTableWidgetItem(playerName));
        table->setItem(row, 2, new QTableWidgetItem("Join"));
    }

    dialog->deleteLater();
    QMessageBox::information(this, "Server Started", "Waiting for players to join...");


}

void LobbyWindow::onJoinGame() {
    if (!connectionManager->initializeClient()) {
        QMessageBox::warning(this, "Error", "Failed to connect to the server.");
        return;
    }

    int selectedRow = table->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "No Lobi Selected", "Please select a lobby to join.");
        return;
    }

    QTableWidgetItem *availabilityItem = table->item(selectedRow, 2);
    if (availabilityItem->text() == "Filled") {
        QMessageBox::information(this, "Lobi Full", "This lobby is already filled.");
        return;
    }

    availabilityItem->setText("Filled");
}


void LobbyWindow::handleGameStart() {
    clientManager = connectionManager->getClientManager();
    qDebug() << "Game is starting."<< clientManager->ClientId;
    gameWindow = new ClientWindow(clientManager, nullptr);
    gameWindow->show();
    close();
}

void LobbyWindow::returnToMenu()
{
    emit backToMenu();
}
